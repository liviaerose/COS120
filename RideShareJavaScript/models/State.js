const { Model } = require("objection");
const { hash, compare } = require("bcrypt");

class State extends Model {
  static get tableName() {
    return "State";
  }
}

module.exports = State;
