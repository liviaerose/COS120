// Knex
const knex = require("knex")({
  client: "pg",
  connection: {
    host: "faraday.cse.taylor.edu", // PostgreSQL server
    user: "jolie_rabideau", // Your user name
    password: "moqonoqo", // Your password
    database: "jolie_rabideau", // Your database name
  },
});

// Objection
const objection = require("objection");
objection.Model.knex(knex);

// Models
const User = require("./models/User");
const Ride = require("./models/Ride");
const Location = require("./models/Location");
const Driver = require("./models/Driver");
const Drivers = require("./models/Drivers");
const Passenger = require("./models/Passenger.js");
const State = require("./models/State.js");
const Vehicle_Type = require("./models/vehicleType.js");
const Vehicle = require("./models/Vehicle.js");

// Hapi
const Joi = require("@hapi/joi"); // Input validation
const Hapi = require("@hapi/hapi"); // Server

const server = Hapi.server({
  host: "localhost",
  port: 3000,
  routes: {
    cors: true,
  },
});

async function init() {
  // Show routes at startup.
  await server.register(require("blipp"));

  // Output logging information.
  await server.register({
    plugin: require("hapi-pino"),
    options: {
      prettyPrint: true,
    },
  });

  // Configure routes.
  server.route([
    {
      method: "GET",
      path: "/users",
      config: {
        description: "Retrieve all users",
      },
      handler: () => User.query(),
    },

    {
      method: "GET", // Get driver collection
      path: "/drivers",
      handler: async (request, h) => {
        return Driver.query();
      },
    },

    {
      method: "GET", // Get passenger collection
      path: "/passengers",
      handler: async (request, h) => {
        return Passenger.query().withGraphFetched("user");
      },
    },
    
    {
      method: "GET",
      path: "/rides",
      config: {
        description: "Retrieve all rides",
      },
      handler: () => Ride.query(),
    },

    {
      method: "GET", // Get vehicle collection
      path: "/vehicles",
      handler: async (request, h) => {
        return Vehicle.query();
      },
    },

    {
      method: "GET", // Get vehicle-type collection
      path: "/vehicle-types",
      handler: async (request, h) => {
        return Vehicle_Type.query();
      },
    },

    {
      method: "GET",
      path: "/rides/{id}",
      config: {
        description: "Retrieve ride by id",
      },
      handler: (request, h) => {
        return User.query().where(request.params.id === User.id)
      },
    },

    {
      method: "GET",
      path: "/locations",
      config: {
        description: "Retrieve all locations",
      },
      handler: () => Location.query(),
    },
    
    {
      method: "GET",
      path: "/users/{id}",
      config: {
        description: "Retrieve users by ID",
        validate: {
          params: Joi.object({
            id: Joi.number().integer().min(1).required()
          })
        }
      },
      handler: (request, h) => {
        return User.query()
          .findById(request.params.id)
          .then(rowsFound => {
            if (rowsFound) {
              return h.response({
                ok: true,
                message: `Found User with ID number: '${request.params.id}'`,
                results: rowsFound
              })
              .code(200);
            } else {
              return h.response({
                ok: false,
                message: `No User with ID number: '${request.params.id}' found`,
              })
              .code(404);
            }
          });
      }
    },

    {
      method: "GET",
      path: "/drivers/{id}",
      options: {
        validate: {
          params: Joi.object({
            id: Joi.number()
              .integer()
              .min(1),
          }),
        },
      },
      handler: async (request, h) => {
        let driver = await Driver.query()
          .where("id", request.params.id)
          .first();
        if (driver) return driver;
        return Boom.notFound(`No driver with ID ${request.params.id}`);
      },
    },

    {
      method: "GET",
      path: "/passengers/{id}",
      options: {
        validate: {
          params: Joi.object({
            id: Joi.number()
              .integer()
              .min(1),
          }),
        },
      },
      handler: async (request, h) => {
        let passenger = await Passenger.query()
          .where("passengerId", request.params.id)
          .first()
          .withGraphFetched("user"); //NOTE should do some more complex data manip for UI here
        if (passenger) return passenger;
        return Boom.notFound(`No passenger with ID ${request.params.id}`);
      },
    },
    
    {
      method: "DELETE",
      path: "/users/{id}",
      config: {
        description: "Delete a user by ID",
        validate: {
          params: Joi.object({
            id: Joi.number().integer().min(1).required()
          })
        }
      },
      handler: (request, h) => {
        return User.query()
          .deleteById(request.params.id)
          .then((rowsDeleted_ => {
            if (rowsDeleted === 1) {
              return {
                ok: true,
                message: `User with ID number: '${request.params.id}' has been deleted`,
              };
            } else {
              return {
                ok: false,
                message: `User with ID number: '${request.params.id}' could not be deleted at this time`,
              };
            }
          }
        ));
      }
    },

    {
      method: "POST",
      path: "/users",
      config: {
        description: "Sign up",
        validate: {
          payload: Joi.object({
            firstName: Joi.string().required(),
            lastName: Joi.string().required(),
            email: Joi.string().email().required(),
            phone: Joi.string().required(),
            password: Joi.string().required(),
          }),
        },
      },
      handler: async (request) => {
        const existingUser = await User.query()
          .where("email", request.payload.email)
          .first();
        if (existingUser) {
          return {
            ok: false,
            msge: `User '${request.payload.email}' already exists`,
          };
        }

        const newUser = await User.query().insert({
          firstName: request.payload.firstName,
          lastName: request.payload.lastName,
          email: request.payload.email,
          phone: request.payload.phone,
          password: request.payload.password,
        });

        if (newUser) {
          return {
            ok: true,
            msge: `Created user '${request.payload.email}'`,
          };
        } else {
          return {
            ok: false,
            msge: `Couldn't create user with email '${request.payload.email}'`,
          };
        }
      },
    },

    {
      method: "POST",
      path: "/login",
      config: {
        description: "Log in",
        validate: {
          payload: Joi.object({
            email: Joi.string()
              .email()
              .required(),
            password: Joi.string()
              .min(8)
              .required(),
          }),
        },
      },
      handler: async (request) => {
        const user = await User.query()
          .where("email", request.payload.email)
          .first();
        if (user && (await user.verifyPassword(request.payload.password))) {
          return {
            ok: true,
            msge: `Logged in successfully as '${request.payload.email}'`,
            details: {
              id: user.id,
              firstName: user.firstName,
              lastName: user.lastName,
              email: user.email,
            },
          };
        } else {
          return {
            ok: false,
            msge: "Invalid email or password",
          };
        }
      },
    },

    {
      method: "POST", // Register user as driver
      path: "/drivers",
      options: {
        validate: {
          payload: Joi.object({
            userId: Joi.number().integer()
              .min(1)
              .max(1000)
              .required(),
            licenseNumber: Joi.string()
              .min(1)
              .max(30)
              .required(),
            licenseState: Joi.string()
              .min(1)
              .max(2)
              .required(),
          }),
        },
      },
      handler: async (request, h) => {
        const existingDriver = await Driver.query()
          .where("userId", request.payload.userId)
          .first();
        if (existingDriver) {
          return {
            ok: false,
            msge: `This user is already a driver '${request.payload.email}'`,
          };        }
        let driver = await Driver.query().insert({
          userId: request.payload.userId,
          licenseNumber: request.payload.licenseNumber,
          licenseState: request.payload.licenseState,
        });

        if (driver) {
          return {
            ok: true,
            msge: `Created account '${request.payload.email}'`,
          };
        } else {
          return {
            ok: false,
            msge: `Couldn't create account with email '${request.payload.email}'`,
          };
        }
      },
    },
  ]);

  // Start the server.
  await server.start();
}

// Go!
init();
