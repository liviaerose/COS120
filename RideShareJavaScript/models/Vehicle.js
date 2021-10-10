const { Model } = require("objection");
const { hash, compare } = require("bcrypt");

class Vehicle extends Model {
  static get tableName() {
    return "Vehicle";
  }
}

module.exports = Vehicle;
