const { Model } = require("objection");
const { hash, compare } = require("bcrypt");

class Driver extends Model {
  static get tableName() {
    return "Driver";
  }
}
module.exports = Driver;
