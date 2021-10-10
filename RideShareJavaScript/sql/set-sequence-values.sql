-- Update sequences to avoid collisions with existing PKs

SELECT setval('account_id_seq', 101, false);
SELECT setval('driver_id_seq', 101, false);
SELECT setval('location_id_seq', 101, false);
SELECT setval('ride_id_seq', 101, false);
SELECT setval('user_id_seq', 101, false);
SELECT setval('vehicle_id_seq', 101, false);
