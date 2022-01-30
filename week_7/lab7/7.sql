SELECT AVG(songs.energy) FROM songs
JOIN artists ON artists.id = artist_id
WHERE artists.name = "Drake";