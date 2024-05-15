// 12S23010 - Tiffani Rumiris Br Butar - Butar
// 12S23024 - Eska Natasia Silaen

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./libs/dorm.h"
#include "./libs/student.h"

int main(int _argc, char **_argv)
{
    Dorm* LEFT = (Dorm*) malloc(sizeof(Dorm));
    strcpy(LEFT->name, "left");
    LEFT->capacity = 10;
    LEFT->gender = GENDER_FEMALE;
    LEFT->residents_num = 0;
    
    Dorm *dorms = NULL;
    struct student_t *students = NULL;
    
    unsigned short totalDorm = 0;
    unsigned short totalStudent = 0;
    
    char line[255];
    char delim[2] = "#";

    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "---") == 0) {
            printf("---\n");
            break;
        }

        if (strcmp(line, "student-print-all") == 0) {
            for (short i = 0; i < totalStudent; i++) {
                printStudent(students[i]);
            }
        } else if (strcmp(line, "dorm-print-all") == 0) {
            for (short i = 0; i < totalDorm; i++) {
                print_dorm(dorms[i]);
            }
        } else if (strcmp(line, "student-print-all-detail") == 0) {
            for (short i = 0; i < totalStudent; i++) {
                printStudentDetails(students[i]);
            }
        } else if (strcmp(line, "dorm-print-all-detail") == 0) {
            for (short i = 0; i < totalDorm; i++) {
                printDormDetails(dorms[i]);
            }
        } else {
            char *token = strtok(line, delim);

            if (strcmp(token, "student-add") == 0) {
                token = strtok(NULL, delim); char *_id = token;
                token = strtok(NULL, delim); char *_name = token;
                token = strtok(NULL, delim); char *_year = token;
                token = strtok(NULL, delim);

                if (totalStudent > 0) {
                    students = (struct student_t*) realloc(students, (totalStudent + 1) * sizeof(struct student_t));
                } else {
                    students = (struct student_t*) malloc(sizeof(struct student_t));
                }

                if (strcmp(token, "male") == 0) {
                    students[totalStudent] = create_student(_id, _name, _year, GENDER_MALE);
                } else if (strcmp(token, "female") == 0) {
                    students[totalStudent] = create_student(_id, _name, _year, GENDER_FEMALE);
                }
                totalStudent++;
            } else if (strcmp(token, "dorm-add") == 0) {
                token = strtok(NULL, delim); char *_name = token;
                token = strtok(NULL, delim); unsigned short _capacity = atoi(token);
                token = strtok(NULL, delim);

                if (totalDorm > 0) {
                    dorms = (Dorm*) realloc(dorms, (totalDorm + 1) * sizeof(Dorm));
                } else {
                    dorms = (Dorm*) malloc(sizeof(Dorm));
                }

                if (strcmp(token, "male") == 0) {
                    dorms[totalDorm] = create_dorm(_name, _capacity, GENDER_MALE);
                } else if (strcmp(token, "female") == 0) {
                    dorms[totalDorm] = create_dorm(_name, _capacity, GENDER_FEMALE);
                }
                totalDorm++;
            } else if (strcmp(token, "assign-student") == 0) {
                token = strtok(NULL, delim); char *_id = token;
                token = strtok(NULL, delim); char *dorm_name = token;

                short studentIdx = findStudentIdx(_id, students, totalStudent);
                short dormIdx = findDormIdx(dorm_name, dorms, totalDorm);

                if (studentIdx >= 0 && dormIdx >= 0) {
                    assign(&students[studentIdx], &dorms[dormIdx]);
                }
            } else if (strcmp(token, "move-student") == 0) {
                token = strtok(NULL, delim); char *_id = token;
                token = strtok(NULL, delim); char *dorm_name = token;

                short studentIdx = findStudentIdx(_id, students, totalStudent);
                short newDormIdx = findDormIdx(dorm_name, dorms, totalDorm);

                if (studentIdx >= 0 && newDormIdx >= 0) {
                    if (students[studentIdx].dorm != NULL) {
                        short oldDormIdx = findDormIdx(students[studentIdx].dorm->name, dorms, totalDorm);
                        if (oldDormIdx >= 0) {
                            moveStudent(&students[studentIdx], &dorms[newDormIdx], &dorms[oldDormIdx]);
                        }
                    } else {
                        assign(&students[studentIdx], &dorms[newDormIdx]);
                    }
                }
            } else if (strcmp(token, "dorm-empty") == 0) {
                token = strtok(NULL, delim);
                char *dorm_name = token;
                short target = findDormIdx(dorm_name, dorms, totalDorm);

                if (target >= 0) {
                    for (short i = 0; i < totalStudent; i++) {
                        if (students[i].dorm != NULL && strcmp(students[i].dorm->name, dorm_name) == 0) {
                            unassign(&students[i], &dorms[target]);
                        }
                    }
                }
            } else if (strcmp(token, "student-leave") == 0) {
                token = strtok(NULL, delim);
                short studentIdx = findStudentIdx(token, students, totalStudent);

                if (studentIdx >= 0 && students[studentIdx].dorm != NULL) {
                    short dormIdx = findDormIdx(students[studentIdx].dorm->name, dorms, totalDorm);
                    if (dormIdx >= 0) {
                        unassign(&students[studentIdx], &dorms[dormIdx]);
                        students[studentIdx].dorm = LEFT;
                    }
                }
            }
        }
    }

    free(dorms);
    free(students);
    free(LEFT);

    return 0;
}
