#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>

char *getmod(mode_t mode,char *line){
    memset(line,0,sizeof(char)*11);
    strcat(line,S_ISDIR(mode)?"d":"-");
    strcat(line,(mode&S_IRWXU)&S_IRUSR?"r":"-");
    strcat(line,(mode&S_IRWXU)&S_IWUSR?"w":"-");
    strcat(line,(mode&S_IRWXU)&S_IXUSR?"x":"-");
    strcat(line,(mode&S_IRWXG)&S_IRGRP?"r":"-");
    strcat(line,(mode&S_IRWXG)&S_IWGRP?"w":"-");
    strcat(line,(mode&S_IRWXG)&S_IXGRP?"x":"-");
    strcat(line,(mode&S_IRWXO)&S_IROTH?"r":"-");
    strcat(line,(mode&S_IRWXO)&S_IWOTH?"w":"-");
    strcat(line,(mode&S_IRWXO)&S_IXOTH?"x":"-");
    return line;
}


void printdir(char *dir, int depth){
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    char *path,fname[512],mod[11],ctm[10];
    struct passwd *user=NULL;
    struct group *mygroup=NULL;
    struct tm *ltm;
    if((dp = opendir(dir)) == NULL){     
        printf("open fail.\n");
        return ;
    }
    chdir(dir);       
    while((entry = readdir(dp)) != NULL){   
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)){
            if(!strcmp(entry->d_name, ".") ||!strcmp(entry->d_name, ".."))
               	continue;
            printf("\n");
            printf("%d\t", depth);
            user=getpwuid(statbuf.st_uid);
            mygroup = getgrgid(statbuf.st_gid);
            printf("%s\t%10s\t%10s\t",getmod(statbuf.st_mode,mod),user->pw_name,mygroup->gr_name);
            printf("%9d\t",statbuf.st_nlink);
            printf("%9d\t",statbuf.st_size);
            ltm=localtime(&statbuf.st_mtime);
            strftime(ctm,9,"%b",ltm);
            printf("%5s",ctm);
            strftime(ctm,9,"%d",ltm);
            printf("%3s",ctm);
            strftime(ctm,9,"%Y",ltm);
            printf("%5s\t",ctm);
            printf("%s\n",entry->d_name);

            printdir(entry->d_name, depth+4);
        }   
	    else{
            printf("%d\t", depth);
            user=getpwuid(statbuf.st_uid);
            mygroup = getgrgid(statbuf.st_gid);
            printf("%s\t%10s\t%10s\t",getmod(statbuf.st_mode,mod),user->pw_name,mygroup->gr_name);
            printf("%9d\t",statbuf.st_nlink);
            printf("%9d\t",statbuf.st_size);
            ltm=localtime(&statbuf.st_mtime);
            strftime(ctm,9,"%b",ltm);
            printf("%5s",ctm);
            strftime(ctm,9,"%d",ltm);
            printf("%3s",ctm);
            strftime(ctm,9,"%Y",ltm);
            printf("%5s\t",ctm);
            printf("%s\n",entry->d_name);
        }
    }
    chdir(".."); 
    closedir(dp); 
}

int main(){
    printdir("..", 0);
    return 0;
}
