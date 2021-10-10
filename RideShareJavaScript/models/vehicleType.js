const { Model } = require("objection");
const { hash, compare } = require("bcrypt");
class VehicleType extends Model {
  static get tableName() {
    return "VehicleType";
  }
}


module.exports = VehicleType;
