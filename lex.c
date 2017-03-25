/* This is a sample Lex compiler
 * created by : Biju Rakhul.S.I
 * 
 * This was done as the first lab experiment of the Compiler Design
 * laboratory.
 * Though I took a week to complete ;)
 */

#include <stdio.h>
#include <string.h>

void isIdent(char * t_token);
void isImmediate(char * t_immediate);
void skipStmts(FILE * dup_file_ptr, char);
void errorHandler(int);
void isOperator(char dup_readChar);
void isStringChar(char dup_readChar, FILE * dup_file_ptr);

char keyword[14][10] = {"int","float","char","unsigned",
						"if" , "while","do" ,"for",
						"signed","short","long","main",
						"void","return"
					   };

int main(int argc, char * argv[]){

	FILE * file_ptr;
	char readChar;
	int count = 0, s_flag = 1;
	char token[36];

	if (argc!=2){
		/* check if arguments are given */
		printf("\nArgument error\nSyntax: lex filename.c\n");
	} else {
		
		/* check for valid file name */
		if ((file_ptr = fopen(argv[1], "r")) == NULL){
			printf("\nCannot open file : %s\n", argv[1]);
		} else {
			while((readChar = getc(file_ptr)) != EOF){
				if ((readChar == '\'') || (readChar == '\"')){
					//printf("In printf\n");
					isStringChar(readChar, file_ptr);
					continue;
				}
				if((readChar == '#') || (readChar == '/')){ 
					skipStmts(file_ptr, readChar);
					continue;
				} else {
					if (((isalnum(readChar)) == 0) && (readChar != ' ') && (readChar != '\n') && (readChar != '_')){
						isOperator(readChar);
						if (strlen(token) != 0){
							readChar = ' ';
						} else
							continue;
					}
				}
				if((readChar != ' ') && (readChar != ';') && (readChar != '\n')){
					token[count] = readChar;
					count++;
				} else {
					token[count] = '\0';
					isIdent(token);
					for (count = 0; count < 36; token[count++]=0);
                                        count = 0;
				}
			}	
		}
	}		
	return 0;
}

/* check if the string is a valid Identifier and if so, check
 * for keyword */
void isIdent(char * t_token){
        int i_flag = 0, countx = 0;
        char * dup_t_token = t_token;
	while (*t_token != '\0' && (isdigit(t_token[0]) == 0)){
		if ((isalnum(*t_token) != 0)){
			i_flag = 1;
		} else {
			if (*t_token == '_'){
				t_token++;
				continue;
			}
			i_flag = 0;
			break;
		}
		t_token++;
	}
	if (i_flag == 1){
		t_token = dup_t_token;
		while (countx < 14){
			if (strcmp(t_token, &keyword[countx][0]) == 0){
				printf("\nKeyword    : %s", dup_t_token);
				countx = -1;
				break;
			}
			countx++;
		}
		if (countx != -1)
			printf("\nIdentifier : %s", dup_t_token);
	} else {
		isImmediate(dup_t_token);
        }
}

/* check for constant */
void isImmediate(char * t_immediate){
	char * dup_t_immediate = t_immediate;
	int im_flag = 0;
	if (isdigit(*t_immediate)){
		while(*t_immediate != '\0'){
			if ((isdigit(*t_immediate))){
				t_immediate++;
				im_flag = 1;
			} else {
				im_flag = 0;
				break;
			}
		}
	}
	if (im_flag == 1)
		printf("\nConstant   : %s", dup_t_immediate);
}

/* check for operators */
void isOperator(char dup_readChar){
	char op_table[11] = {'+','-','/','%','*','|','&','~','<','>','='};
	int count=0;
	while (count < 11){
		if (dup_readChar == op_table[count]){
			printf("\nOperator   : %c", dup_readChar);
			count = -1;
			break;
		}
		++count;
	}
	if (count != -1)
		printf("\nSpecial    : %c", dup_readChar);
}

/* skip all preprocessor statements, comments */
void skipStmts(FILE * dup_file_ptr, char dup_readChar){
	if (dup_readChar == '#'){
		while ((dup_readChar = getc(dup_file_ptr)) != '\n');
	} else {
		if (dup_readChar == '/'){
			if ((dup_readChar = getc(dup_file_ptr)) == '/')
				while ((dup_readChar = getc(dup_file_ptr)) != '\n');
			else {
				if (dup_readChar == '*' ){
					while (((dup_readChar = getc(dup_file_ptr)) != '*') && (dup_readChar != EOF));
					if (dup_readChar = getc(dup_file_ptr) == '/')
						;
					else
						errorHandler(1);
				}
			}
		}
	}
}

/* handle character related constants */
void isStringChar(char dup_readChar, FILE * dup_file_ptr){
	char charGarage[50];
	int count = 0, str_flag = 0;
	if (dup_readChar == '\''){
		while ((dup_readChar = getc(dup_file_ptr)) != EOF && (count < 2)){
			charGarage[count] = dup_readChar;
			if ((dup_readChar == '\'') && (charGarage[0] != '\'')){
				str_flag = 2;
				charGarage[1] = '\0';
				break;
			}
			count++;
		}
	} else {
		while ((dup_readChar = getc(dup_file_ptr)) != EOF && (count < 50)){
			if (dup_readChar != '\"'){
				charGarage[count] = dup_readChar;
			} else {
				charGarage[count] = '\0';
				str_flag = 1;
				break;
			}
			count++;
		}
	}
	if (str_flag == 1)
		printf("\nString     : %s", charGarage);
	else if (str_flag == 2)
		printf("\nchar const : %c", charGarage[0]);
	else
		errorHandler(2);
}

/* handle errors returned by functions */
void errorHandler(int errorNum){
	switch (errorNum){
		case 1:
			printf("\n[error]: '*' or '/' missing\n");
			break;
		case 2:
			printf("\n[error]: string or char literal missing \' \" \' \n");
			break;
		default:
			printf("[error]: unspecified error\n");
	}
}
