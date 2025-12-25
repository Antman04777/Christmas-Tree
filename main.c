#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

static unsigned char *christmas_tree = NULL;
static size_t tree_file_size = 0;
static unsigned int flash_speed = 0;

#define ANSI_COLOR_BLUE    "\x1b[38;5;21m"
#define ANSI_COLOR_GREEN   "\x1b[38;5;28m"
#define ANSI_COLOR_BROWN   "\x1b[38;5;94m"
#define ANSI_COLOR_RED     "\x1b[38;5;124m"
#define ANSI_COLOR_ORANGE  "\x1b[38;5;214m"
#define ANSI_COLOR_YELLOW  "\x1b[38;5;226m"
#define ANSI_COLOR_WHITE   "\x1b[38;5;231m"
#define ANSI_COLOR_BLACK   "\x1b[38;5;232m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define INVALID_NUM        0

void
init_tree()
{
	FILE *tree_file = fopen("tree.txt", "r");
	fseek(tree_file, 0L, SEEK_END);
	tree_file_size = ftell(tree_file);
	fseek(tree_file, 0L, SEEK_SET);
	christmas_tree = malloc(tree_file_size);
	fread(christmas_tree, sizeof(unsigned char), tree_file_size, tree_file);
	fclose(tree_file);
}

void
clear_terminal()
{
	printf("\x1B[39;49m");
	printf("\x1B[2J");
	printf("\x1B[3J");
	printf("\x1B[H");
}

void
draw_tree()
{
	static bool star_flashing = false;
	
	star_flashing = !star_flashing;
	
	for ( size_t tree_idx = 0; tree_idx < tree_file_size; ++tree_idx )
	{
		unsigned char tree_char = *(christmas_tree + tree_idx);
		float random_number = (float) rand() / RAND_MAX;

		if ( tree_char == 'o' )
		{
			if ( star_flashing )
			{
				if ( random_number < 0.25f )
					printf(ANSI_COLOR_ORANGE "%c" ANSI_COLOR_RESET, tree_char);
				else if ( random_number < 0.5f && random_number > 0.25f )
					printf(ANSI_COLOR_RED "%c" ANSI_COLOR_RESET, tree_char);
				else if ( random_number < 0.75f && random_number > 0.5f )
					printf(ANSI_COLOR_GREEN "%c" ANSI_COLOR_RESET, tree_char);
				else
					printf(ANSI_COLOR_BLUE "%c" ANSI_COLOR_RESET, tree_char);



			}
			else
				printf(ANSI_COLOR_BLACK "%c" ANSI_COLOR_RESET, tree_char);

		}
		else if ( tree_char == '*' )
		{
			if ( star_flashing )
				printf(ANSI_COLOR_YELLOW "%c" ANSI_COLOR_RESET, tree_char);
			else
				printf(ANSI_COLOR_BLACK "%c" ANSI_COLOR_RESET, tree_char);
				
		}
		else if ( tree_char == '|' )
		{
			printf(ANSI_COLOR_BROWN "%c" ANSI_COLOR_RESET, tree_char);
		}
		else
		{
			printf(ANSI_COLOR_WHITE "%c" ANSI_COLOR_RESET, tree_char);
		}
	}
}

size_t
get_string_length( const char *string )
{
	size_t string_length = 0;

	int string_idx = 0;
	while ( *(string + string_idx) != '\0' )
	{
		string_length ++;
		string_idx ++;
	}
	
	return string_length;
}

int
power ( int number, int exponent )
{
	int new_number = number;

	for ( int i = 0; i < exponent - 1; i++ )
		new_number *= number;

	return new_number;
}

int
string_to_int ( const char *string )
{
	size_t string_length = get_string_length(string);
	int string_num = 0;

	for ( size_t i = 0; i < string_length; ++i )
	{
		int digit = *(string + i) - 48;
		if ( digit < 0 || digit > 9 )
			return INVALID_NUM;

		string_num += digit * power(10, string_length - i - 1);
	}

	return string_num;
}

int
main ( int argc, char **argv )
{
	switch ( argc )
	{
		case 1:
			flash_speed = 500000;
			break;
		case 2:
			flash_speed = string_to_int(argv[1]) * 1000;
			if ( flash_speed == INVALID_NUM )
			{
				fprintf(stderr, "Usage: %s [flash speed in milliseconds]\n", argv[0]);
				exit(-1);
			}
			break;
		default:
			fprintf(stderr, "Usage: %s [flash speed in milliseconds]\n", argv[0]);
			exit(-1);
	}
	
	srand(time(NULL));
	init_tree();
	clear_terminal();

	while ( true )
	{
		draw_tree();
		usleep(flash_speed);
		fflush(stdout);
		clear_terminal();
	}

	free(christmas_tree);
	
	return 0;
}
