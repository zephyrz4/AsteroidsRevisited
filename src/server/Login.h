/*
 * Login.h
 *
 *  Created on: Jul 28, 2013
 *      Author: cameron
 */

#ifndef LOGIN_H_
#define LOGIN_H_

#include <vector>
#include <string>
#include "User.h"

enum authenticationTypes {
	VALID,
	UNKNOWN_USER,
	INVALID_PASSWORD

};

enum registerTypes {

	REG_SUCCESS,
	REG_INUSE,
	REG_FAIL,

};

class Login {
public:
	Login();
	virtual ~Login();

	int authenticateUser(std::string username, std::string password);
	int registerUser(std::string username, std::string password);

private:

	std::vector<User> users;
};

#endif /* LOGIN_H_ */