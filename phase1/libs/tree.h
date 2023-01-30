int make_branch(int size){

    int count = strlen(output) + 2 * size + 2;

    if(count > last_count){
        output = realloc(output,sizeof(char) * count);
        last_count = count;
    }

    for(int i = 0;i < size -1;i++){
        strcat(output," ");
    }
    strcat(output,"|\n");
    for(int i = 0;i < size -1;i++){
        strcat(output," ");
    }
    strcat(output,"|_");
}

int edit_branch(int line, int end_line, int size){
    long long count = strlen(output);
    int l = 1;
    int pos = 1;
    for(int i = 0; i < count;i++){
        if(output[i] == '\n'){
            l++;
            pos = 1;
            continue;
        }
        if(pos == size && line < l && l <= end_line){
            output[i] = '|';
        }
        if(pos == size && l >= end_line){
            return 1;
        }
        pos++;
    }
    return 1;
}

int make_tree(int depth,int line,int size,char * p_path,int tree){
    
    int end_line = line;
    char path[200];
    
    if(tree == depth){
        return line;
    }
    
    if(depth == 0){
        size = strlen("root:");
        strcat(output,"root:\n");
        
        struct dirent *files;
        DIR *dir = opendir("./root");
        
        
        while ((files = readdir(dir)) != NULL){
            char * name = files->d_name;
            
            if(name[0] != '.'){
                char path_of_dir[200];
                strcpy(path_of_dir,"./root/");
                strcat(path_of_dir, name);
                make_branch(size);
                

                struct stat buf;
                stat(path_of_dir, &buf);

                strcat(output,name);
                if(S_ISDIR(buf.st_mode)) {
                    strcat(output, ":");
                }
                strcat(output,"\n");

                if(end_line != line){
                    edit_branch(line,end_line,size);
                    line = end_line;
                }

                line +=2;

                if(S_ISDIR(buf.st_mode)) {
                    end_line = make_tree(depth + 1,line,size + strlen(name) + 1,path_of_dir,tree);
                }else{
                    end_line+=2;
                }
            }

        }
        closedir(dir);

        return 1;
    }else{
        strcpy(path,p_path);
        
        struct dirent *files;
        DIR *dir = opendir(path);
        
        while ((files = readdir(dir)) != NULL){
            char * name = files->d_name;
            if(name[0] != '.'){

                char path_of_dir[200];
                strcpy(path_of_dir,path);
                strcat(path_of_dir,"/");
                strcat(path_of_dir, name);

                make_branch(size);

                struct stat buf;
                stat(path_of_dir, &buf);
                strcat(output,name);
                
                

                if(S_ISDIR(buf.st_mode)) {
                    strcat(output, ":");
                }
                strcat(output,"\n");
                if(end_line != line){
                    edit_branch(line,end_line,size);
                    line = end_line;
                }

                line +=2;
                
                if(S_ISDIR(buf.st_mode)) {
                    end_line = make_tree(depth + 1,line,size + strlen(name) + 1,path_of_dir,tree);
                }else{
                    end_line+=2;
                }
            }

        }
        closedir(dir);
        return end_line;
    }


    
}

int tree(char str[]){

    char * token = strtok(str," ");
    token = strtok(NULL," ");
    if(token == NULL){
        return -1;
    }
    int depth = atoi(token);
    if(depth == 0 && token[0] != '0'){
        return -1;
    }
    if(depth < -1){
        return -12;
    }
    if(depth == 0){
        strcpy(output,"root:\n\0");
        return 1;
    }
    char p_path[200] ="\0";
    output[0] = '\0';
    make_tree(0,1,0,p_path,depth);
    return 1;
}
