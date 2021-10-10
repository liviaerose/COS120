const { Model } = require("objection");
const { hash, compare } = require("bcrypt");

class Ride extends Model {
  static get tableName() {
    return "Ride";
  }

  static get relationMappings() {
    const Location = require("./Location.js");
    const Vehicle = require("./Vehicle.js");
    const Passenger = require("./Passenger.js");
    const Drivers = require("./Drivers.js");

    return {
      toLocation: {
        relation: Model.BelongsToOneRelation,
        modelClass: Location,
        join: {
          from: "Ride.toLocationId",
          to: "Location.id",
        },
      },

      fromLocation: {
        relation: Model.BelongsToOneRelation,
        modelClass: Location,
        join: {
          from: "Ride.fromLocationId",
          to: "Location.id",
        },
      },

      vehicle: {
        relation: Model.BelongsToOneRelation,
        modelClass: Vehicle,
        join: {
          from: "Ride.vehicleId",
          to: "Vehicle.id",
        },
      },

      passenger: {
        relation: Model.HasManyRelation,
        modelClass: Passenger,
        join: {
          from: "Ride.id",
          to: "Passenger.rideId",
        },
      },

      drivers: {
        relation: Model.HasManyRelation,
        modelClass: Drivers,
        join: {
          from: "Ride.id",
          to: "Drivers.rideId",
        },
      },
    };
  }
}

module.exports = Ride;
