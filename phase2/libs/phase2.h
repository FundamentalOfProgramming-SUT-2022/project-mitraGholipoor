#include "phase2/vim.h"
#include "phase2/win_layout.h"
#include "phase2/insert.h"
#include "phase2/remove.h"
#include "phase2/selection.h"
#include "phase2/mode.h"
#include "phase2/copy.h"
#include "phase2/cut.h"
#include "phase2/paste.h"
#include "phase2/save.h"
#include "phase2/open.h"
#include "phase2/replace.h"
#include "phase2/auto-indent.h"
#include "phase2/arman.h"
#include "phase2/undo.h"
#include "phase2/find.h"



void vim_save_output_in_vim(){
    
        FILE * fp;
        fp = fopen(vim_save_address,"w");
        if(output != NULL && strlen(output) != 0){
            fputs(output,fp);
        }
        fclose(fp);

    
}

void handel_phase1_command_1(){
    if(has_name){
        vim_save_form_file(vim_address);

    }
    set_str_from_vim();
    vim_diff = 0;
    vim_make_screen();
    vim_change_mode_to_normal(0);
}

void handel_phase1_command_2(){
    if(has_name){
        vim_save_form_vim(vim_address);
    }
    vim_save_output_in_vim();
    set_str_from_vim();
    vim_address[0] = '\0';
    vim_bs_name[0] = '\0';
    has_name = 0;
    vim_save = 0;
    vim_diff = 0;
    vim_make_screen();
    vim_change_mode_to_normal(0);
}

void vim_handle_err(int err){
    switch (err){
        case -1:
            strcpy(vim_massage,"command is not valid\0");
            break;
        case -2:
            strcpy(vim_massage,"file is already exist\0");
            break;
        case -3:
            strcpy(vim_massage,"file path is not valid\0");
            break;
        case -4:
            strcpy(vim_massage,"invalid directory or file name\0");
            break;
        case -5:
            strcpy(vim_massage,"invalid position\0");
            break;
        case -6:
            strcpy(vim_massage,"file does not exist\0");
            break;
        case -7:
            strcpy(vim_massage,"number of pos is out of range\0");
            break;
        case -8:
            strcpy(vim_massage,"number of line is out of range\0");
            break;
        case -9:
            strcpy(vim_massage,"invalid size\0");
            break;
        case -10:
            strcpy(vim_massage,"invalid pattern\0");
            break;
        case -11:
            strcpy(vim_massage,"two file are empty\0");
            break;
        case -12:
            strcpy(vim_massage,"invalid depth\0");
            break;
        case -13:
            strcpy(vim_massage,"patten not match\0");
            break;
        case -14:
            strcpy(vim_massage,"permission denied\0");
            break;
        case -15:
            strcpy(vim_massage,"please set a name for file with saveas command\0");
            break;
        default:
            break;
    }
    clear();
    set_str_from_vim();
    vim_make_screen();
    vim_change_mode_to_normal(1);
    refresh();
}

void vim_check_the_command(char str[]){
    char copy[1000];
    strcpy(copy,str);
    char * token = strtok(copy," ");
    char * check_arman = strstr(str," =D ");
    if(check_arman == NULL){
        if(strcmp(token,"createfile") == 0){
            int err = create_file(str);
            if(err != 1){
                vim_handle_err(err);
            }else{
                strcpy(vim_massage,"file created successfully!\0");
            }
        }else if(strcmp(token,"insertstr") == 0){
            int err = insert_str(str,0);
            if (err != 1)
            {
                vim_handle_err(err);
            }else{
                handel_phase1_command_1();
                strcpy(vim_massage,"string inserted successfully!\0");
                vim_change_mode_to_normal(1);
            }
            
        }else if(strcmp(token,"cat") == 0){
            int err = cat(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_2();
            }
            
        }else if(strcmp(token,"removestr") == 0){
            int err = remove_str(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_1();
                strcpy(vim_massage,"removed successfully!\0");
                vim_change_mode_to_normal(1);
            }
            
        }else if(strcmp(token,"copystr") == 0){
            int err = copy_str(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                strcpy(vim_massage,"copied to clipboard successfully!\0");
                vim_change_mode_to_normal(1);
            }
            
        }else if(strcmp(token,"cutstr") == 0){
            int err = cut_str(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_1();
                strcpy(vim_massage,"cut to clipboard successfully!\0");
                vim_change_mode_to_normal(1);
            }
            
        }else if(strcmp(token,"pastestr") == 0){
            int err = paste_str(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_1();
                strcpy(vim_massage,"paste successfully!\0");
                vim_change_mode_to_normal(1);
            }
            
        }else if(strcmp(token,"find") == 0){
            int err = find_str(str,0);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_2();
            }
            
        }else if(strcmp(token,"replace") == 0){
            int err = vim_replace(str,0);
            if (err != 1){
                vim_handle_err(err);
            }else{
                strcpy(vim_massage,"replaced successfully!\0");
                vim_change_mode_to_normal(1);
            }
            
        }else if(strcmp(token,"undo") == 0){
            int err = vim_undo(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                strcpy(vim_massage,"done!\0");
                vim_change_mode_to_normal(1);
            }
            
        }else if(strcmp(token,"grep") == 0){
            int err = grep(str,0);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_2();
            }
            
        }else if(strcmp(token,"compare") == 0){
            int err = compare_files(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_2();
            }
        }else if(strcmp(token,"tree") == 0){
            int err = tree(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_2();
            }
        }else if(strcmp(token,"auto-indent") == 0){
            int err = auto_indent(str);
            if (err != 1){
                vim_handle_err(err);
            }else{
                handel_phase1_command_1();
                strcpy(vim_massage,"done!\0");
                vim_change_mode_to_normal(1);
            }
            
        }else if(strcmp(token,"saveas") == 0){
            int err = vim_saveas(str);
            if(err != 1){
                vim_handle_err(err);
            }else{
                strcat(vim_massage,"file saved successfully!");
                vim_change_mode_to_normal(1);
            }
        }else if(strcmp(token,"save") == 0){
            int err = vim_save_file(str);
            if(err != 1){
                vim_handle_err(err);
            }else{
                strcat(vim_massage,"file saved successfully!");
                vim_change_mode_to_normal(1);
            }
        }else if(strcmp(token,"open") == 0){
            int err = vim_open(str);
            if(err != 1){
                vim_handle_err(err);
            }else{
                vim_change_mode_to_normal(0);
            }
        }else{
            vim_handle_err(-1);
        }
    }else{
        int err = vim_handle_arman(str);
        if(err != 1){
            vim_handle_err(err);
        }
    }
}


