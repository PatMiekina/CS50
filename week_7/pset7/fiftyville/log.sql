-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Check crime reports from day of theft
SELECT * FROM crime_scene_reports
WHERE
    year = 2020
    AND month = 7
    AND day = 28;

-- 295 | 2020 | 7 | 28 | Chamberlin Street | Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse. Interviews

-- Read interviews from day of theft
SELECT * FROM interviews
WHERE
    year = 2020
    AND month = 7
    AND day = 28;

-- 161 | Ruth | 2020 | 7 | 28 | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- 162 | Eugene | 2020 | 7 | 28 | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- 163 | Raymond | 2020 | 7 | 28 | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.

-- Check courthouse video logs
SELECT * FROM courthouse_security_logs
WHERE
    year = 2020
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute >= 15
    AND minute <= 25;



-- Check license plate of the car leaving at 10:25am

-- 260 | 2020 | 7 | 28 | 10 | 16 | exit | 5P2BI95
-- 261 | 2020 | 7 | 28 | 10 | 18 | exit | 94KL13X
-- 262 | 2020 | 7 | 28 | 10 | 18 | exit | 6P58WS2
-- 263 | 2020 | 7 | 28 | 10 | 19 | exit | 4328GD8
-- 264 | 2020 | 7 | 28 | 10 | 20 | exit | G412CB7
-- 265 | 2020 | 7 | 28 | 10 | 21 | exit | L93JTIZ
-- 266 | 2020 | 7 | 28 | 10 | 23 | exit | 322W7JE
-- 267 | 2020 | 7 | 28 | 10 | 23 | exit | 0NTHK55
-- 268 | 2020 | 7 | 28 | 10 | 35 | exit | 1106N58

SELECT * FROM people
WHERE
    license_plate = "5P2BI95"
    OR license_plate = "94KL13X"
    OR license_plate = "6P58WS2"
    OR license_plate = "4328GD8"
    OR license_plate = "G412CB7"
    OR license_plate = "L93JTIZ"
    OR license_plate = "322W7JE"
    OR license_plate = "0NTHK55"
    OR license_plate = "1106N58";

-- Check who the license plates belonged to

--     id | name | phone_number | passport_number | license_plate
-- 221103 | Patrick | (725) 555-4692 | 2963008352 | 5P2BI95
-- 243696 | Amber | (301) 555-4174 | 7526138472 | 6P58WS2
-- 396669 | Elizabeth | (829) 555-5269 | 7049073643 | L93JTIZ
-- 398010 | Roger | (130) 555-0289 | 1695452385 | G412CB7
-- 449774 | Madison | (286) 555-6063 | 1988161715 | 1106N58
-- 467400 | Danielle | (389) 555-5198 | 8496433585 | 4328GD8
-- 514354 | Russell | (770) 555-1861 | 3592750733 | 322W7JE
-- 560886 | Evelyn | (499) 555-9472 | 8294398571 | 0NTHK55
-- 686048 | Ernest | (367) 555-5533 | 5773159633 | 94KL13X

-- Check calls under minute at the time of the event 10:15am

SELECT caller, receiver, duration FROM phone_calls
WHERE
    year = 2020
    AND month = 7
    AND day = 28
    AND duration <= 60;

-- (130) 555-0289 | (996) 555-8899 | 51
-- (499) 555-9472 | (892) 555-8872 | 36
-- (367) 555-5533 | (375) 555-8161 | 45
-- (609) 555-5876 | (389) 555-5198 | 60
-- (499) 555-9472 | (717) 555-1342 | 50
-- (286) 555-6063 | (676) 555-6554 | 43
-- (770) 555-1861 | (725) 555-3243 | 49
-- (031) 555-6622 | (910) 555-3251 | 38
-- (826) 555-1652 | (066) 555-9701 | 55
-- (338) 555-6650 | (704) 555-2131 | 54

-- Find the destination of the earliest next day flight (as per interviews suggestion)

SELECT city FROM airports
WHERE id IN(SELECT destination_airport_id FROM flights
        WHERE
            day = "29"
            AND month = "7"
            AND year = "2020"
        ORDER BY
            hour,
            minute
        LIMIT 1);

-- London!!!

-- Select people on the plane who:
-- had under 1 min calls,
-- were in the courthouse about 10 mins after the theft and
-- were on the plane

SELECT name FROM people
WHERE passport_number IN(SELECT passport_number FROM passengers
    WHERE flight_id IN (SELECT id FROM flights
        WHERE
            day = "29"
            AND month = "7"
            AND year = "2020"
        ORDER BY
            hour,
            minute
        LIMIT 1))
AND phone_number IN (SELECT caller FROM phone_calls
WHERE
    year = 2020
    AND month = 7
    AND day = 28
    AND duration <= 60)
AND license_plate IN(SELECT license_plate FROM courthouse_security_logs
WHERE
    year = 2020
    AND month = 7
    AND day = 28
    AND hour = 10
    AND minute >= 15
    AND minute <= 25)
AND id IN(SELECT person_id FROM bank_accounts
    WHERE account_number IN(
        SELECT account_number FROM atm_transactions
        WHERE
            year = 2020
            AND month = 7
            AND day = 28
            AND atm_location = "Fifer Street"
            AND transaction_type = "withdraw"));

-- Ernest


-- check calls with Ernest to find the accomplice

SELECT name FROM people
WHERE
    phone_number IN (SELECT receiver FROM phone_calls
        WHERE
            year = 2020
            AND month = 7
            AND day = 28
            AND duration <= 60
            AND caller IN(SELECT phone_number FROM people
                WHERE name = "Ernest"));

-- Berthold
