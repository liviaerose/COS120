const { Model } = require("objection");
const { hash, compare } = require("bcrypt");

class Drivers extends Model {
  static get tableName() {
    return "Drivers";
  }
}

module.exports = Drivers;
