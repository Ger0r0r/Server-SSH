#include "../headers/server.h"

//compile with -lpam -lpam_misc

struct pam_conv my_conv = {
	misc_conv,
	NULL,
};

int Login_into_user(char *username){
	pam_handle_t *pam;
	int ret;

	ret = pam_start("my_ssh", username, &my_conv, &pam);

	if (ret != PAM_SUCCESS){
		printf("Failed pam_start\n");
		return -1;
	}

	ret = pam_authenticate(pam, 0);

	if (ret != PAM_SUCCESS){
		printf("Incorrect password!\n");
		return -1;
	}

	ret = pam_acct_mgmt(pam, 0);

	if (ret != PAM_SUCCESS){
		printf("User account expired!\n");
		return -1;
	}

	if (pam_end(pam, ret) != PAM_SUCCESS){
		printf("Unable to pam_end()\n");
		return -1;
	}

	printf("Login succesfull\n");
	return 0;
}

int User_login(char * username){
	struct passwd *info;

	info = getpwnam(username);

	if (!info){
		perror("getpwnam");
		return -1;
	}

	system("cat /proc/self/status | grep CapEff");

	printf("name: %s, uid: %d\n", info->pw_name, info->pw_uid);

	if (Login_into_user(username)){
		printf("Unsuccessful authentification for user %s\n", username);
		return -1;
	}

	if (setgid(info->pw_gid)){
		perror("setgid");
		return -1;
	}

	if (setuid(info->pw_uid)){
		perror("setuid");
		return -1;
	}
}