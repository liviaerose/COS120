const { Model } = require("objection");
const { hash, compare } = require("bcrypt");

class Passenger extends Model {
  static get tableName() {
    return "Passenger";
  }
}

module.exports = Passenger;
