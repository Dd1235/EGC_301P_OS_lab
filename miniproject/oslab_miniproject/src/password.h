// password.h
#ifndef PASSWORD_H
#define PASSWORD_H

// Allows the logged-in user to change their password
void change_user_password(int client_sock, const char *username, const char *role);

#endif
