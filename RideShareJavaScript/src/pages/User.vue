<template>
  <v-container>
    <div>
      <h4 class="display-1">Account Information</h4>
      
      <v-data-table
        class="elevation-1"
        v-bind:headers="headers"
        v-bind:items="users"
      >
        <template v-slot:item="{ item }">
         <!-- <tr v-bind:class="itemClass(item)"> -->
            <td>{{ item.email }}</td>
            <td>{{ item.firstName }}</td>
            <td>{{ item.lastName }}</td>
            <td>{{ item.phoneNumber }}</td>
            <td>
              <v-icon @click="deleteUser(item)">
                mdi-delete
              </v-icon>
              <v-icon small class="ml-2" @click="updateUser(item)">
                mdi-arrow
              </v-icon>
            </td>
        <!--  </tr> -->
        </template>
      </v-data-table>

      <v-snackbar v-model="snackbar.show">
        {{ snackbar.text }}
        <v-btn color="red" text @click="snackbar.show = false">
          Close
        </v-btn>
      </v-snackbar>
    </div>

  </v-container>
</template>

<script>
export default {
  name: "User",
  data: function() {
    return {
      headers: [
        { text: "Email", value: "email" },
        { text: "First", value: "firstName" },
        { text: "Last", value: "lastName" },
        { text: "Phone Number", value: "phoneNumber" }
      ],
      users: [],
      snackbar: {
        show: false,
        text: ""
      }
    };
  },
  mounted: function() {
    this.$axios.get("/users").then(response => {
      this.users = response.data.map(user => ({
        id: user.id,
        email: user.email,
        firstName: user.first_name,
        lastName: user.last_name,
        phoneNumber: user.phone_number
      }));
    });
  },
  methods: {
    // Display a snackbar message.
    showSnackbar(text) {
      this.snackbar.text = text;
      this.snackbar.show = true;
    },
   
    updateUser(item) {
      console.log("UPDATE", JSON.stringify(item, null, 2));
      this.showSnackbar("Cannot update.");
    },
    deleteAccount(item) {
      this.$axios.delete(`/users/${item.id}`).then(response => {
        if (response.status == 200) {
          this.users = this.users.filter(
            user => user.id !== item.id
          );
        }
      });
    }
  }
};
</script>

<style>
.currentUser {
  background: grey;
}
</style>
