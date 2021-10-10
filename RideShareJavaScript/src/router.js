import Vue from "vue";
import Router from "vue-router";

import Home from "./pages/Home.vue";
import SignIn from "./pages/SignIn.vue";
import SignUp from "./pages/SignUp.vue";
import About from "./pages/About.vue";
import User from "./pages/User.vue";

Vue.use(Router);

export default new Router({
  mode: "history",
  base: process.env.BASE_URL,
  routes: [
    { name: "home-page", path: "/", component: Home },
    { name: "sign-in", path: "/sign-in", component: SignIn },
    { name: "sign-up", path: "/sign-up", component: SignUp },
    { name: "about-us", path: "/about-us", component: About },
    { name: "user", path: "/user", component: User }
  ]
});
