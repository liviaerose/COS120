const { Model } = require("objection");
const { hash, compare } = require("bcrypt");

class Location extends Model {
  static get tableName() {
    return "Location";
  }

  static get relationMappings() {
    const Ride = require("./Ride.js");
    const State = require("./State.js");

    return {
      ride: {
        relation: Model.HasManyRelation,
        modelClass: Ride,
        join: {
          from: "Location.id",
          to: "Ride.fromLocationId",
        },
        join: {
          from: "Location.id",
          to: "Ride.toLocationId",
        },
      },
      state: {
        relation: Model.BelongsToOneRelation,
        modelClass: State,
        join: {
          from: "Location.id",
          to: "State.abbreviation",
        },
      },
    };
  }

  getState() {
    return this.$relatedQuery("state")
      .select("name")
      .then((theName) => {
        return theName;
      })
      .catch((err) => {
        console.log(err);
      });
  }
}

module.exports = Location;
