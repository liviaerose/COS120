CREATE TABLE account
(
    id         serial  NOT NULL,
    first_name varchar NOT NULL,
    last_name  varchar NOT NULL,
    email      varchar NOT NULL,
    password   varchar NOT NULL,
    CONSTRAINT account_pk
        PRIMARY KEY (id)
);

COMMENT ON TABLE account IS 'User Account';

CREATE TABLE state
(
    abbreviation char(2)     NOT NULL,
    name         varchar(40) NOT NULL,
    CONSTRAINT state_pkey
        PRIMARY KEY (abbreviation)
);

CREATE TABLE "vehicleType"
(
    id   serial      NOT NULL,
    type varchar(80) NOT NULL,
    CONSTRAINT "vehicleType_pkey"
        PRIMARY KEY (id)
);

CREATE TABLE vehicle
(
    id              serial           NOT NULL,
    make            varchar(40)      NOT NULL,
    model           varchar(40)      NOT NULL,
    color           varchar(40)      NOT NULL,
    "vehicleTypeId" integer          NOT NULL,
    capacity        integer          NOT NULL,
    mpg             double precision NOT NULL,
    "licenseState"  char(2)          NOT NULL,
    "licensePlate"  varchar(40)      NOT NULL,
    CONSTRAINT vehicle_pkey
        PRIMARY KEY (id),
    CONSTRAINT "vehicle_vehicleType_id_fk"
        FOREIGN KEY ("vehicleTypeId") REFERENCES "vehicleType",
    CONSTRAINT vehicle_state_abbreviation_fk
        FOREIGN KEY ("licenseState") REFERENCES state
);

CREATE TABLE location
(
    id        serial      NOT NULL,
    name      varchar(40) NOT NULL,
    address   varchar(80) NOT NULL,
    city      varchar(40) NOT NULL,
    state     char(2)     NOT NULL,
    "zipCode" varchar(40) NOT NULL,
    CONSTRAINT location_pk
        PRIMARY KEY (id),
    CONSTRAINT location_state_abbreviation_fk
        FOREIGN KEY (state) REFERENCES state
);

CREATE TABLE "user"
(
    id          serial                NOT NULL,
    "firstName" varchar(40)           NOT NULL,
    "lastName"  varchar(40)           NOT NULL,
    email       varchar(80)           NOT NULL,
    password    varchar               NOT NULL,
    phone       integer               NOT NULL,
    "isAdmin"   boolean DEFAULT FALSE NOT NULL,
    CONSTRAINT user_pkey
        PRIMARY KEY (id)
);

CREATE TABLE driver
(
    id              serial      NOT NULL,
    "userId"        integer     NOT NULL,
    "licenseNumber" varchar(40) NOT NULL,
    "licenseState"  char(2)     NOT NULL,
    CONSTRAINT driver_pkey
        PRIMARY KEY (id),
    CONSTRAINT driver_user_id_fk
        FOREIGN KEY ("userId") REFERENCES "user",
    CONSTRAINT driver_state_abbreviation_fk
        FOREIGN KEY ("licenseState") REFERENCES state
);

CREATE TABLE ride
(
    id                 serial           NOT NULL,
    date               date             NOT NULL,
    "numberPassengers" integer          NOT NULL,
    time               time             NOT NULL,
    distance           double precision NOT NULL,
    "fuelPrice"        double precision NOT NULL,
    fee                double precision NOT NULL,
    charge             double precision NOT NULL,
    "vehicleId"        integer          NOT NULL,
    "fromLocationId"   integer          NOT NULL,
    "toLocationId"     integer          NOT NULL,
    CONSTRAINT ride_pkey
        PRIMARY KEY (id),
    CONSTRAINT ride_vehicle_id_fk
        FOREIGN KEY ("vehicleId") REFERENCES vehicle,
    CONSTRAINT ride_location_id_fk
        FOREIGN KEY ("fromLocationId") REFERENCES location,
    CONSTRAINT ride_location_id_fk_2
        FOREIGN KEY ("toLocationId") REFERENCES location
);

CREATE TABLE drivers
(
    "driverId" integer NOT NULL,
    "rideId"   integer NOT NULL,
    CONSTRAINT drivers_pk
        PRIMARY KEY ("driverId", "rideId"),
    CONSTRAINT drivers_driver_id_fk
        FOREIGN KEY ("driverId") REFERENCES driver,
    CONSTRAINT drivers_ride_id_fk
        FOREIGN KEY ("rideId") REFERENCES ride
);

CREATE TABLE "authorization"
(
    "driverId"  integer NOT NULL,
    "vehicleId" integer NOT NULL,
    CONSTRAINT authorization_pk
        PRIMARY KEY ("vehicleId", "driverId"),
    CONSTRAINT authorization_driver_id_fk
        FOREIGN KEY ("driverId") REFERENCES driver,
    CONSTRAINT authorization_vehicle_id_fk
        FOREIGN KEY ("vehicleId") REFERENCES vehicle
);

CREATE TABLE passenger
(
    "passengerId" integer NOT NULL,
    "rideId"      integer NOT NULL,
    CONSTRAINT passenger_pk
        PRIMARY KEY ("passengerId", "rideId"),
    CONSTRAINT passenger_user_id_fk
        FOREIGN KEY ("passengerId") REFERENCES "user",
    CONSTRAINT passenger_ride_id_fk
        FOREIGN KEY ("rideId") REFERENCES ride
);

