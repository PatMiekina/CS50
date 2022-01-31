import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    sum_of_shares_price = 0
    # Query for data from the database
    rows = db.execute("""SELECT symbol, SUM(shares) FROM stocks
                            JOIN users ON id == user_id
                            WHERE user_id == :user_id
                            AND symbol != "CASH"
                            GROUP BY symbol
                            HAVING SUM(shares) > 0""", user_id=session["user_id"])

    # Get the quote
    for row in rows:
        quoted = lookup(row["symbol"])
        price = quoted["price"]
        total_price = quoted["price"] * row["SUM(shares)"]
        row["name"] = quoted["name"]
        row["price"] = usd(quoted["price"])
        row["total_price"] = usd(total_price)
        sum_of_shares_price += total_price

    # Add total cash
    funds = db.execute("SELECT cash FROM users WHERE id == :user_id", user_id=session["user_id"])
    cash = usd(funds[0]["cash"])

    total_value = funds[0]["cash"] + sum_of_shares_price
    total_value = usd(total_value)
    return render_template("index.html", rows=rows, cash=cash, total_value=total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        quoted = lookup(symbol)
        if not quoted:
            return apology("Invalid Symbol.")

        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Shares must be an integer!")

        if shares <= 0:
            return apology("Shares must be a positive integer!")
        if not shares:
            return apology("Share must be minimum 1.")

        if isinstance(shares, str):
            return apology("Share must be numeric.")

        shares = int(shares)
        funds = db.execute("SELECT cash FROM users WHERE id == :user_id", user_id=session["user_id"])
        total = quoted["price"]*shares
        cash = funds[0]["cash"]

        if total > cash:
            return apology("Not enough cash!.")

        cash = cash - total
        db.execute("UPDATE users SET cash = :cash WHERE id == :user_id", cash=cash, user_id=session["user_id"])

        time = datetime.datetime.now()
        time = time.strftime("%Y-%m-%d %H:%M:%S")

        db.execute("INSERT INTO stocks VALUES (:user_id, :symbol, :shares, :time, :price, :total)",
                   user_id=session["user_id"], symbol=symbol,
                   shares=shares,
                   time=time,
                   price=quoted["price"], total=total)

        flash("Bought succesfully.")
        return redirect("/")


@app.route("/history")
@login_required
def history():

    # Select all information about transactions
    rows = db.execute("""SELECT * FROM stocks
                            JOIN users ON id == user_id
                            WHERE user_id == :user_id""", user_id=session["user_id"])

    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    # Generic template
    if request.method == "GET":
        return render_template("quote.html")

    # When user searches for a symbol
    else:
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)
        # Wrong symbol
        if not quoted:
            return apology("Invalid Symbol")
        return render_template("quoted.html", name=quoted["name"], symbol=quoted["symbol"], price=usd(quoted["price"]))

    # Other problems
    return apology("Quote is not working.")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        if not username:
            return apology("You must provide a username.")
        username_check = db.execute("SELECT * FROM users WHERE username = :username",
                                    username=request.form.get("username"))
        if len(username_check) != 0:
            return apology("This username is already taken.")

        password = request.form.get("password")
        if not password:
            return apology("You must provide a password.")

        confirmation = request.form.get("confirmation")
        if not confirmation or password != confirmation:
            return apology("You must provide a confirmation password.")
        if password != confirmation:
            return apology("The password do not match.")

        # Hash the given password for security
        hash = generate_password_hash(password)

        # Insert the hashed password to users table
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=hash)

        user = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        session["user_id"] = user[0]["id"]

        flash("Registered")
        return redirect("/")

    return apology("Register is not working.")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        # Select all stocks owned by the current user
        rows = db.execute("""SELECT symbol FROM stocks
                                JOIN users ON id == user_id
                                WHERE user_id == :user_id
                                AND symbol != "CASH"
                                GROUP BY symbol
                                HAVING SUM(shares) > 0""", user_id=session["user_id"])

        return render_template("sell.html", rows=rows)
    else:
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")
        quoted = lookup(symbol)
        # Wrong symbol
        if not quoted:
            return apology("Invalid Symbol.")
        # User doesn't have enough shares
        if not shares:
            return apology("You must sell minimum 1 share.")

        # Collect the total
        rows = db.execute("""SELECT SUM(shares) FROM stocks
                                JOIN users ON id == user_id
                                WHERE user_id == :user_id
                                AND symbol == :symbol
                                GROUP BY symbol""", user_id=session["user_id"], symbol=symbol)

        current_shares = float(rows[0]["SUM(shares)"])
        shares = float(shares)

        # Check if enough shares to sell
        if shares > current_shares:
            return apology("You don't have so much shares.")

        total_sell_price = float(quoted["price"]) * float(shares)

        # Calculate total funds
        funds = db.execute("SELECT cash FROM users WHERE id == :user_id", user_id=session["user_id"])
        cash = float(funds[0]["cash"])

        # Calculate total price
        cash = cash + total_sell_price
        db.execute("UPDATE users SET cash = :cash WHERE id == :user_id", cash=cash, user_id=session["user_id"])

        # Add a timestamp
        time = datetime.datetime.now()
        time = time.strftime("%Y-%m-%d %H:%M:%S")

        db.execute("INSERT INTO stocks VALUES (:user_id, :symbol, :shares, :time, :unit_price, :total_price)",
                   user_id=session["user_id"], symbol=symbol,
                   shares=-shares, time=time,
                   unit_price=-float(quoted["price"]), total_price=-total_sell_price)

        flash("Sold.")
        return redirect("/")

    return apology("Sell is not working.")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
