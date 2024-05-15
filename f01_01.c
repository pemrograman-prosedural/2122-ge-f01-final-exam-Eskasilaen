// 12S23010 - Tiffani Rumiris Br Butar - Butar
// 12S23024 - Eska Natasia Silaen

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include "./libs/dorm.h"
#include "./libs/student.h"

int main(int _argc, char **_argv)
{
    dorm* LEFT = (Dorm*) malloc(sizeof(Dorm));
    strcpy(LEFT->name,"left");
    LEFT->capacity = 10;
    LEFT->gender = GENDER_FEMALE;
    LEFT->residents_num = 0;
    dorm *dorms = (Dorm*) malloc(1 * sizeof(Dorm));
    student *students = (Student*) malloc(1 * sizeof(Student));
    unsigned short zdrm;
    unsigned short zstd;
    char line[255];
    char delimiter[2] = "#";


    while ( 1 ) {
        line[0] = '\0';
        fgets(line, 255, stdin);
        fflush(stdin);
        
        /* declared in scope level --> temporary
           but the result remains */
        {
            int len = strlen(line);
            for (short a = 0; a < len; a++) {
                if(line[a] == '\r' || line[a] == '\n') {
                    for(short b = a; b < len; b++) { line[b] = line[b + 1]; }
                    len--;
                    a--;
                }
            }
        }

        if ( strcmp(line, "---") == 0 ) break;

        else if ( strcmp(line, "student-print-all") == 0 ) {
            for (short i=0; i<zstd; i++) {
                print_student(students[i]);
            }
        }

        else if ( strcmp(line, "dorm-print-all") == 0 ) {
            for (short i=0; i<zdrm; i++) {
                print_dorm(dorms[i]);
            }
        }

        else if ( strcmp(line, "student-print-all-detail") == 0 ) {
            for (short i=0; i<zstd; i++) {
                print_student_details(students[i]);
            }
            
        }

        else if ( strcmp(line, "dorm-print-all-detail") == 0 ) {
            for (short i=0; i<zdrm; i++) {
                print_dorm_details(dorms[i]);
            }
        }

        else {
            char *token = strtok(line, delimiter);

            if ( strcmp(token, "student-add") == 0 ) {
                token = strtok(NULL, delimiter); char *_id = token;
                token = strtok(NULL, delimiter); char *_name = token;
                token = strtok(NULL, delimiter); char *_year = token;
                token = strtok(NULL, delimiter);
                if ( zstd > 0 ) {
                    students = (Student*) realloc(students, (zstd + 1) * sizeof(Student));
                }
                if ( strcmp(token, "male") == 0 ) {
                    students[zstd] = create_student(_id, _name, _year, GENDER_MALE);
                    zstd++;
                }
                else if ( strcmp(token, "female") == 0 ) {
                    students[zstd] = create_student(_id, _name, _year, GENDER_FEMALE);
                    zstd++;
                }
            }

            else if ( strcmp(token, "dorm-add") == 0 ) {
                token = strtok(NULL, delimiter); char *_name = token;
                token = strtok(NULL, delimiter); unsigned short _capacity = atoi(token);
                token = strtok(NULL, delimiter);
                if ( zdrm > 0 ) {
                    dorms = (Dorm*) realloc(dorms, (zdrm+1) * sizeof(Dorm));
                }
                if ( strcmp(token, "male") == 0 ) {
                    dorms[zdrm] = create_dorm(_name, _capacity, GENDER_MALE);
                    zdrm++;
                }
                else if ( strcmp(token, "female") == 0 ) {
                    dorms[zdrm] = create_dorm(_name, _capacity, GENDER_FEMALE);
                    zdrm++;
                }
            }

            else if ( strcmp(token, "assign-student") == 0 ) {
                token = strtok(NULL, delimiter); char *_id = token;
                token = strtok(NULL, delimiter); char *dorm_name = token;

                short student_id = find_id(_id, students, zstd);
                short dorm_id = find_id(dorm_name, dorms, zdrm);

                if ( student_id>=0 && dorm_id>=0 ) {
                    assign(&students[student_id], &dorms[dorm_id]);
                }
            }

            else if ( strcmp(token, "move-student") == 0 ) {
                token = strtok(NULL, delimiter); char *_id = token;
                token = strtok(NULL, delimiter); char *dorm_name = token;

                short student_id = find_id(_id, students, zstd);
                short newdorm = find_dorm(dorm_name, dorms, zdrm);
                char *_name = students[student_id].dorm->name;
                if (_name != NULL) {
                    short olddorm = find_id( _name, dorms, zdrm );

                    if ( studentIdx>=0 && newdorm>=0 && olddorm>=0 ) {
                        move_student(&students[student_id], &dorms[newdorm] , &dorms[olddorm]);
                    }
                } else {
                    assign_student(&students[student_id], &dorms[newdorm]);
                }
            }

            else if ( strcmp(token, "dorm-empty") == 0 ) {
                token = strtok(NULL, delimiter);
                char *dorm_name = token;
                short target = find_dorm(dorm_name, dorms, zdrm);
                
                for (short i=0; i<totalStudent; i++) {
                    if (students[i].dorm != NULL) {     
                        if ( strcmp(students[i].dorm->name, dorm_name) == 0 ) {
                            unassign_student(&students[i], &dorms[target]);
                        }
                    }
                }
            }

            else if ( strcmp(token, "student-leave") == 0 ) {
                token = strtok(NULL, delimiter);
                short student_id = find_id(token, students, zstd);
                short dorm_id = find_dorm(students->dorm->name, dorms, zdrm);
                unassign(&students[student_id], &dorms[dorm_id]);
                students[student_id].dorm = LEFT;
            }
        }
    }
    
    free(dorms);
    free(students);
    free(LEFT);
    
    return 0;
}

