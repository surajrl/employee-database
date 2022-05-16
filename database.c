#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* maximum number of employees that can be stored at once (relevant only
   to storage using an array) */
#define MAX_EMPLOYEES 200

#define MAX_NAME_LENGTH 100
#define MAX_JOB_LENGTH  100

/* Employee structure
 */
struct Employee
{
   /* Employee details */
   char name[MAX_NAME_LENGTH+1]; /* name string */
   char sex;   /* sex identifier, either 'M' or 'F' */
   int  age;                     /* age */
   char job[MAX_JOB_LENGTH+1];   /* job string */
   
   /* pointers to previous and next employee structures in the linked list
      (for if you use a linked list instead of an array) */
   struct Employee *prev, *next;
};

/* Array of employees */
static struct Employee employee_array[MAX_EMPLOYEES];

/* Number of Employees stored */
static int no_employees = 0;


/* read_line():
 *
 * Read line of characters from file pointer "fp", copying the characters
 * into the "line" string, up to a maximum of "max_length" characters, plus
 * one for the string termination character '\0'. Reading stops upon
 * encountering the end-of-line character '\n', for which '\0' is substituted
 * in the string. If the end of file character EOF is reached before the end
 * of the line, the failure condition (-1) is returned. If the line is longer
 * than the maximum length "max_length" of the string, the extra characters
 * are read but ignored. Success is returned (0) on successfully reading
 * a line.
 */
static int read_line ( FILE *fp, char *line, int max_length )
{
   int i;
   char ch;

   /* initialize index to string character */
   i = 0;

   /* read to end of line, filling in characters in string up to its
      maximum length, and ignoring the rest, if any */
   for(;;)
   {
      /* read next character */
      ch = fgetc(fp);

      /* check for end of file error */
      if ( ch == EOF )
	 return -1;

      /* check for end of line */
      if ( ch == '\n' )
      {
	 /* terminate string and return */
	 line[i] = '\0';
	 return 0;
      }

      /* fill character in string if it is not already full*/
      if ( i < max_length )
	 line[i++] = ch;
   }

   /* the program should never reach here */
   return -1;
}

/* read_string():
 *
 * Reads a line from the input file pointer "fp", starting with the "prefix"
 * string, and filling the string "string" with the remainder of the contents
 * of the line. If the start of the line does not match the "prefix" string,
 * the error condition (-1) is returned. Having read the prefix string,
 * read_string() calls read_line() to read the remainder of the line into
 * "string", up to a maximum length "max_length", and returns the result.
 */
static int read_string ( FILE *fp,
			 char *prefix, char *string, int max_length )
{
   int i;

   /* read prefix string */
   for ( i = 0; i < strlen(prefix); i++ )
      if ( fgetc(fp) != prefix[i] )
	 /* file input doesn't match prefix */
	 return -1;

   /* read remaining part of line of input into string */
   return ( read_line ( fp, string, max_length ) );
}

/* Reads employee and returns the employee. */
static struct Employee read_employee(void)
{
   char user_input[101];

   char name[MAX_NAME_LENGTH+1];
   char sex;
   int age;
   char job[MAX_JOB_LENGTH+1];

   int name_done = 0;
   int sex_done = 0;
   int age_done = 0;
   int job_done = 0;

   /* Prompts user for name and checks if data is valid. */
   while (name_done != 1)
   {
      fprintf(stderr, "\nEnter the name in the form <surname>, <other names>: ");
      read_line(stdin, name, MAX_NAME_LENGTH);

      if (strlen(name) == 0)
      {
         fprintf(stderr, "Invalid name.");
         continue;
      }

      name_done = 1;
   }
   
   /* Prompts user for sex and checks if data is valid. */
   while (sex_done != 1)
   {
      fprintf(stderr, "Enter your sex, either 'M' or 'F': ");
      read_line(stdin, user_input, 100);

      if (strcmp(user_input, "M") != 0 && strcmp(user_input, "F") != 0)
      {
         fprintf(stderr, "Invalid sex.\n");
         continue;
      }

      sscanf(user_input, "%c", &sex);
      sex_done = 1;
   }

   /* Prompts user for age and checks if data is valid. */
   while (age_done != 1)
   {
      fprintf(stderr, "Enter your age: ");
      read_line(stdin, user_input, 100);

      if (sscanf(user_input, "%d", &age) != 1)
      {
         fprintf(stderr, "Invalid age.\n");
         continue;
      }

      if (age <= 0)
      {
         fprintf(stderr, "Invalid age.\n");
         continue;
      }

      sscanf(user_input, "%d", &age);
      age_done = 1;
   }
   
   /* Prompts user for job and checks if data is valid. */
   while (job_done != 1)
   {
      fprintf(stderr, "Enter your job: ");
      read_line(stdin, job, MAX_NAME_LENGTH);

      if (strlen(job) == 0)
      {
         fprintf(stderr, "Invalid job.\n");
         continue;
      }

      job_done = 1;
   }
   
   struct Employee new_employee; /* Creates structure to hold the data */
   
   strcpy(new_employee.name, name);
   new_employee.sex = sex;
   new_employee.age = age;
   strcpy(new_employee.job, job);

   return new_employee;
}

/* Store employee in the database. */
static void store_employee(struct Employee add_employee)
{
   employee_array[no_employees] = add_employee;
   no_employees++;
}

/* Sorts the database. */
static void sort_database()
{
   char temp_string[101]; /* Variable to hold the temporary strings */
   int temp_int; /* Variable to hold the temporary integers */
   int i, sorted;

   /* Shaker sort */
   do
   {
      for (i = 1, sorted = 1; i < no_employees; i++)
      {
         if (strcmp(employee_array[i-1].name, employee_array[i].name) > 0)
         {
            sorted = 0;
            strcpy(temp_string, employee_array[i-1].name);
            strcpy(employee_array[i-1].name, employee_array[i].name);
            strcpy(employee_array[i].name, temp_string);

            temp_int = employee_array[i-1].sex;
            employee_array[i-1].sex = employee_array[i].sex;
            employee_array[i].sex = temp_int;

            temp_int = employee_array[i-1].age;
            employee_array[i-1].age = employee_array[i].age;
            employee_array[i].age = temp_int;

            strcpy(temp_string, employee_array[i-1].job);
            strcpy(employee_array[i-1].job, employee_array[i].job);
            strcpy(employee_array[i].job, temp_string);
         }
      }

      for (i = no_employees-1, sorted = 1; i >= 0; i--)
      { 
         if (strcmp(employee_array[i].name, employee_array[i-1].name) < 0)
         {
            sorted = 0;
            strcpy(temp_string, employee_array[i-1].name);
            strcpy(employee_array[i-1].name, employee_array[i].name);
            strcpy(employee_array[i].name, temp_string);

            temp_int = employee_array[i-1].sex;
            employee_array[i-1].sex = employee_array[i].sex;
            employee_array[i].sex = temp_int;

            temp_int = employee_array[i-1].age;
            employee_array[i-1].age = employee_array[i].age;
            employee_array[i].age = temp_int;

            strcpy(temp_string, employee_array[i-1].job);
            strcpy(employee_array[i-1].job, employee_array[i].job);
            strcpy(employee_array[i].job, temp_string);
         }
      }
   } while (!sorted);
}

/* Add new employee to database. */
static void menu_add_employee(void)
{
   /* Read employee from stdin */
   struct Employee new_employee; /* Creates a structure to hold the new employee */
   new_employee = read_employee(); /* Asigns the employee entered to this structure */

   /* Store the new employee in the database */
   store_employee(new_employee); /* New employee is passed as an argument */
}

/* Print database of employees to standard output. */
static void menu_print_database(void)
{
   int i;

   /* Sorts the database by alphabetical order */
   sort_database();

   /* Prints each information of every employee */
   for(i = 0; i < no_employees; i++)
   {
      printf("\nName: %s\n", employee_array[i].name);
      printf("Sex: %c\n", employee_array[i].sex);
      printf("Age: %d\n", employee_array[i].age);
      printf("Job: %s\n", employee_array[i].job);
   }
}	    

/* Delete employee from database. */
static void menu_delete_employee(void)
{
   char name_delete[MAX_NAME_LENGTH+1];
   int name_done = 0;
   int i;

   /* Read name from user input and check if data is valid */
   while (name_done != 1)
   {
      fprintf(stderr, "\nEnter the full name of the employee to be deleted in the form <surname>, <other names>: ");
      read_line(stdin, name_delete, MAX_NAME_LENGTH);

      if (strlen(name_delete) == 0)
      {
         fprintf(stderr, "Invalid name.");
         continue;
      }

      name_done = 1;
   }

   /* Goes through every name in the database to see if the name is there */
   for (i = 0; i < no_employees; i++)
   {
      if (strcmp(employee_array[i].name, name_delete) != 0)
      {
         fprintf(stderr, "Employee not found.\n");
         continue;
      }  

      /* Deletes the data if name is found by moving the element to the 
         end of the array and reducing the length of the array by one */
      if (strcmp(employee_array[i].name, name_delete) == 0) 
      {
         strcpy(employee_array[i].name, employee_array[no_employees+1].name);
         employee_array[i].sex = employee_array[no_employees+1].sex;
         employee_array[i].age = employee_array[no_employees+1].age;
         strcpy(employee_array[i].job, employee_array[no_employees+1].job);

         no_employees--;
      }
   }
}

/* Read file containing database of employees. */
static void read_employee_database ( char *file_name )
{
   FILE *pfile;

   char input[101];
   int i;

   char name[MAX_NAME_LENGTH+1];
   char sex;
   int age;
   char job[MAX_JOB_LENGTH+1];

   pfile = fopen(file_name, "r");

   /* Reads each employee */
   for (i = 0; i < 10; i++)
   {
      /* Reads the name and checks if it is valid */
      read_string(pfile, "Name: ", name, MAX_NAME_LENGTH);
      if (strlen(name) == 0)
      {
         fprintf(stderr, "Error: Invalid name found in the database.");
         exit(EXIT_FAILURE);
      }
      
      /* Reads the sex and checks if it is valid */
      read_string(pfile, "Sex: ", input, 101);
      sscanf(input, "%c", &sex);
      if (strcmp(input, "M") != 0 && strcmp(input, "F") != 0)
      {
         fprintf(stderr, "Error: Invalid sex found in the database.\n");
         exit(EXIT_FAILURE);
      }

      /* Reads the age and checks if it is valid */
      read_string(pfile, "Age: ", input, 101);
      sscanf(input, "%d", &age);
      if (age <= 0)
      {
         fprintf(stderr, "Error: Invalid age found in the database.\n");
         exit(EXIT_FAILURE);
      }
   
      /* Reads the job and checks if it is valid */
      read_string(pfile, "Job: ", job, MAX_JOB_LENGTH);
      if (strlen(job) == 0)
      {
         fprintf(stderr, "Error: Invalid job found in the database..\n");
         exit(EXIT_FAILURE);
      }

      /* Reads the white line and ignores it */
      read_string(pfile, "", input, 101);

      /* Assigns each employee to the database */
      strcpy(employee_array[no_employees].name, name);
      employee_array[no_employees].sex = sex;
      employee_array[no_employees].age = age;
      strcpy(employee_array[no_employees].job, job);
      no_employees++;
   }

   fclose(pfile);
}

/* codes for menu */
#define ADD_CODE    0
#define DELETE_CODE 1
#define PRINT_CODE  2
#define EXIT_CODE   3

int main ( int argc, char *argv[] )
{
   /* check arguments */
   if ( argc != 1 && argc != 2 )
   {
      fprintf ( stderr, "Usage: %s [<database-file>]\n", argv[0] );
      exit(-1);
   }

   /* read database file if provided, or start with empty database */
   if ( argc == 2 )
      read_employee_database ( argv[1] );

   for(;;)
   {
      int choice, result;
      char line[301];

      /* print menu to standard error */
      fprintf ( stderr, "\nOptions:\n" );
      fprintf ( stderr, "%d: Add new employee to database\n", ADD_CODE );
      fprintf ( stderr, "%d: Delete employee from database\n", DELETE_CODE );
      fprintf ( stderr, "%d: Print database to screen\n", PRINT_CODE );
      fprintf ( stderr, "%d: Exit database program\n", EXIT_CODE );
      fprintf ( stderr, "\nEnter option: " );

      if ( read_line ( stdin, line, 300 ) != 0 ) 
         continue;

      result = sscanf ( line, "%d", &choice );
      if ( result != 1 )
      {
	 fprintf ( stderr, "corrupted menu choice\n" );
	 continue;
      }

      switch ( choice )
      {
         case ADD_CODE: /* add employee to database */
	 menu_add_employee();
	 break;

         case DELETE_CODE: /* delete employee from database */
	 menu_delete_employee();
	 break;

         case PRINT_CODE: /* print database contents to screen
			     (standard output) */
	 menu_print_database();
	 break;

	 /* exit */
         case EXIT_CODE:
	 break;

         default:
	 fprintf ( stderr, "illegal choice %d\n", choice );
	 break;
      }

      /* check for exit menu choice */
      if ( choice == EXIT_CODE )
	 break;
   }

   return 0;   
}
