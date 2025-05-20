// auth_utils.h
#ifndef AUTH_UTILS_H
#define AUTH_UTILS_H

// Authenticate a user with given role, username, and password
// Returns 1 on success, 0 on failure
int authenticate(const char *role, const char *username, const char *password);

#endif
