#include "libs/phase1.h"

//---------- #PHASE1 ----------//


void check_the_command(char str[]){
    char copy[1000];
    strcpy(copy,str);
    char * token = strtok(copy," ");
    char * check_arman = strstr(str," =D ");
    if(check_arman == NULL){
        if(strcmp(token,"createfile") == 0){
            int err = create_file(str);
            if(err != 1){
                handle_err(err);
            }else{
                printf("file created successfully!\n");
            }
        }else if(strcmp(token,"insertstr") == 0){
            int err = insert_str(str,0);
            if (err != 1)
            {
                handle_err(err);
            }else{
                printf("string inserted successfully!\n");
            }
            
        }else if(strcmp(token,"cat") == 0){
            int err = cat(str);
            if (err != 1)
            {
                handle_err(err);
            }else{
                printf("%s\n",output);
            }
            
        }else if(strcmp(token,"removestr") == 0){
            int err = remove_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("removed successfully!\n");
            }
            
        }else if(strcmp(token,"copystr") == 0){
            int err = copy_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("copied to clipboard successfully!\n");
            }
            
        }else if(strcmp(token,"cutstr") == 0){
            int err = cut_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("cut to clipboard successfully!\n");
            }
            
        }else if(strcmp(token,"pastestr") == 0){
            int err = paste_str(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("paste successfully!\n");
            }
            
        }else if(strcmp(token,"find") == 0){
            int err = find_str(str,0);
            if (err != 1){
                handle_err(err);
            }else{
                printf("%s\n",output);
            }
            
        }else if(strcmp(token,"replace") == 0){
            int err = replace_str(str,0);
            if (err != 1){
                handle_err(err);
            }else{
                printf("replaced successfully!\n");
            }
            
        }else if(strcmp(token,"undo") == 0){
            int err = undo(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("done!\n");
            }
            
        }else if(strcmp(token,"grep") == 0){
            int err = grep(str,0);
            if (err != 1){
                handle_err(err);
            }else{
                printf("%s",output);
            }
            
        }else if(strcmp(token,"compare") == 0){
            int err = compare_files(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("%s",output);
            }
        }else if(strcmp(token,"tree") == 0){
            int err = tree(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("%s",output);
            }
        }else if(strcmp(token,"auto-indent") == 0){
            int err = auto_indent(str);
            if (err != 1){
                handle_err(err);
            }else{
                printf("done!\n");
            }
            
        }else{
            handle_err(-1);
        }
    }else{
        int err = handle_arman(str);
        if(err != 1){
            handle_err(err);
        }
    }
}

int main(){

    char add[1000];
    char str[1000];
    make_here_root();
    set_clipboard();

    fgets(str,1000,stdin);
    
    while (strcmp(str,"stop\n") != 0){
        
        if(strlen(str) < 2){
            handle_err(-1);
            fgets(str,1000,stdin);
            continue;
        }
        if(str[strlen(str)-1] == '\n'){
            str[strlen(str)-1] = '\0';
        }
    
        check_the_command(str);
        fgets(str,1000,stdin);
    }
    
    return 0;
}