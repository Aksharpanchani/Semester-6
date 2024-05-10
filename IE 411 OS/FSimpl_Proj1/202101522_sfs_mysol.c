#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define BLOCK_SUPER 0
#define BLOCK_BLOCK_BITMAP 1
#define BLOCK_INODE_BITMAP 2
#define BLOCK_INODE_TABLE 3
#define BLOCK_MAX 99
#define INODE_MAX 127

// structure of an inode entry
typedef struct
{
	char TT[2];				  // entry type; "DI" means directory and "FI" means file
	char XX[2], YY[2], ZZ[2]; // the blk for this entry; 00 means not used
} _inode_entry;

// structure of a directory entry
typedef struct
{
	char F;			 // '1' means used; '0' means unused
	char fname[252]; // name of this entry; remember to include null character into it
	char MMM[3];	 // inode entry index which holds more info about this entry
} _directory_entry;

// SFS metadata; read during mounting
int BLB;						// total number of blk
int INB;						// total number of entries in inode table
char _block_bitmap[1024];		// the block bitmap array
char _inode_bitmap[1024];		// the inode bitmap array
_inode_entry _inode_table[128]; // the inode table containing 128 inode entries

// useful info
int free_disk_blocks;					   // number of available disk blk
int free_inode_entries;					   // number of available entries in inode table
int CD_INODE_ENTRY = 0;					   // index of inode entry of the current directory in the inode table
char current_working_directory[252] = "/"; // name of current directory (useful in the prompt)

FILE *df = NULL; // THE DISK FILE

// function declarations
// HELPERS
int stoi(char *, int);
void itos(char *, int, int);
void printPrompt();

// DISK ACCESS
void mountSFS();
int readSFS(int, char *);
int writeSFS(int, char *);

// BITMAP ACCESS
int getBlock();
void returnBlock(int);
int getInode();
void returnInode(int);

// COMMANDS
void ls();
void rd();
void cd(char *);
void md(char *);
void stats();
void create(char *);

/*############################################################################*/
/****************************************************************************/
/* returns the integer value of string s; -1 on error
/*
/****************************************************************************/

int stoi(char *s, int n)
{
	int i;
	int ret = 0;

	for (i = 0; i < n; i++)
	{
		if (s[i] < 48 || s[i] > 57)
			return -1; // non-digit
		ret += pow(10, n - i - 1) * (s[i] - 48);
	}

	return ret;
}

/****************************************************************************/
/* returns the string representation of num in s
/* n is the width of the number; 0 padded if required
/*
/****************************************************************************/

void itos(char *s, int num, int n)
{
	char st[1024];
	sprintf(st, "%0*d", n, num);
	strncpy(s, st, n);
}

/****************************************************************************/
/* prints a prompt with current working directory
/*
/****************************************************************************/

void printPrompt()
{
	printf("SFS::%s# ", current_working_directory);
}

/*############################################################################*/
/****************************************************************************/
/* reads SFS metadata into memory structures
/*
/****************************************************************************/

void mountSFS()
{
	int i;
	char buffer[1024];

	df = fopen("sfs.disk", "r+b");
	if (df == NULL)
	{
		printf("Disk file sfs.disk not find.\n");
		exit(1);
	}

	// read superblock
	fread(buffer, 1, 1024, df);
	BLB = stoi(buffer, 3);
	INB = stoi(buffer + 3, 3);

	// read block bitmap
	fread(_block_bitmap, 1, 1024, df);
	// initialize number of free disk blk
	free_disk_blocks = BLB;
	for (i = 0; i < BLB; i++)
		free_disk_blocks -= (_block_bitmap[i] - 48);

	// read inode bitmap
	fread(_inode_bitmap, 1, 1024, df);
	// initialize number of unused inode entries
	free_inode_entries = INB;
	for (i = 0; i < INB; i++)
		free_inode_entries -= (_inode_bitmap[i] - 48);

	// read the inode table
	fread(_inode_table, 1, 1024, df);
}

/****************************************************************************/
/* reads a block of data from disk file into buffer
/* returns 0 if invalid block number
/*
/****************************************************************************/

int readSFS(int block_number, char buffer[1024])
{

	if (block_number < 0 || block_number > BLOCK_MAX)
		return 0;

	if (df == NULL)
		mountSFS(); // trying to read without mounting...!!!

	fseek(df, block_number * 1024, SEEK_SET); // set file pointer at right position
	fread(buffer, 1, 1024, df);				  // read a block, i.e. 1024 bytes into buffer

	return 1;
}

/****************************************************************************/
/* writes a block of data from buffer to disk file
/* if buffer is null pointer, then writes all zeros
/* returns 0 if invalid block number
/*
/****************************************************************************/

int writeSFS(int block_number, char buffer[1024])
{
	char empty_buffer[1024];

	if (block_number < 0 || block_number > BLOCK_MAX)
		return 0;

	if (df == NULL)
		mountSFS(); // trying to write without mounting...!!!

	fseek(df, block_number * 1024, SEEK_SET); // set file pointer at right position

	if (buffer == NULL)
	{ // if buffer is null
		memset(empty_buffer, '0', 1024);
		fwrite(empty_buffer, 1, 1024, df); // write all zeros
	}
	else
		fwrite(buffer, 1, 1024, df);

	fflush(df); // making sure disk file is always updated

	return 1;
}

/*############################################################################*/
/****************************************************************************/
/* finds the first available block using the block bitmap
/* updates the bitmap
/* writes the block bitmap to disk file
/* returns -1 on error; otherwise the block number
/*
/****************************************************************************/

int getBlock()
{
	int i;

	if (free_disk_blocks == 0)
		return -1;

	for (i = 0; i < BLB; i++)
		if (_block_bitmap[i] == '0')
			break; // 0 means available

	_block_bitmap[i] = '1';
	free_disk_blocks--;

	writeSFS(BLOCK_BLOCK_BITMAP, _block_bitmap);

	return i;
}

/****************************************************************************/
/* updates block bitmap when a block is no longer used
/* blk 0 through 3 are treated special; so they are always in use
/*
/****************************************************************************/

void returnBlock(int index)
{
	if (index > 3 && index <= BLOCK_MAX)
	{
		_block_bitmap[index] = '0';
		free_disk_blocks++;

		writeSFS(BLOCK_BLOCK_BITMAP, _block_bitmap);
	}
}

/****************************************************************************/
/* finds the first unused position in inode table using the inode bitmap
/* updates the bitmap
/* writes the inode bitmap to disk file
/* returns -1 if table is full; otherwise the position
/*
/****************************************************************************/

int getInode()
{
	int i;

	if (free_inode_entries == 0)
		return -1;

	for (i = 0; i < INB; i++)
		if (_inode_bitmap[i] == '0')
			break; // 0 means available

	_inode_bitmap[i] = '1';
	free_inode_entries--;

	writeSFS(BLOCK_INODE_BITMAP, _inode_bitmap);

	return i;
}

/****************************************************************************/
/* updates inode bitmap when an inode entry is no longer used
/*
/****************************************************************************/

void returnInode(int index)
{
	if (index > 0 && index <= INODE_MAX)
	{
		_inode_bitmap[index] = '0';
		free_inode_entries++;

		writeSFS(BLOCK_INODE_BITMAP, _inode_bitmap);
	}
}

/*############################################################################*/
/****************************************************************************/
/* makes root directory the current directory
/*
/****************************************************************************/

void rd()
{
	CD_INODE_ENTRY = 0; // first inode entry is for root directory
	current_working_directory[0] = '/';
	current_working_directory[1] = 0;
}

/****************************************************************************/
/* lists all files and directories in the current directory
/*
/****************************************************************************/

void ls()
{
	char it;
	int blk[3];
	_directory_entry _directory_entries[4];

	int total_files = 0, total_dirs = 0;

	int i, j;
	int enod;

	// read inode entry for current directory
	// in SFS, an inode can point to three blk at the most
	it = _inode_table[CD_INODE_ENTRY].TT[0];
	blk[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blk[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blk[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

	// its a directory; so the following should never happen
	if (it == 'F')
	{
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}

	// lets traverse the directory entries in all three blk
	for (i = 0; i < 3; i++)
	{
		if (blk[i] == 0)
			continue; // 0 means pointing at nothing

		readSFS(blk[i], (char *)_directory_entries); // lets read a directory entry; notice the cast

		// so, we got four possible directory entries now
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0')
				continue; // means unused entry

			enod = stoi(_directory_entries[j].MMM, 3); // this is the inode that has more info about this entry

			if (_inode_table[enod].TT[0] == 'F')
			{ // entry is for a file
				printf("%.252s\t", _directory_entries[j].fname);
				total_files++;
			}
			else if (_inode_table[enod].TT[0] == 'D')
			{ // entry is for a directory; print it in BRED
				printf("\e[1;31m%.252s\e[;;m\t", _directory_entries[j].fname);
				total_dirs++;
			}
		}
	}

	printf("\n%d file%c and %d director%s.\n", total_files, (total_files <= 1 ? 0 : 's'), total_dirs, (total_dirs <= 1 ? "y" : "ies"));
}

/****************************************************************************/
/* moves into the directory <dname> within the current directory if
/* it exists
/*
/****************************************************************************/

void cd(char *dname)
{
	char it;
	int blk[3];
	_directory_entry _directory_entries[4];

	int i, j;
	int enod;

	char find = 0;

	// read inode entry for current directory
	// in SFS, an inode can point to three blk at the most
	it = _inode_table[CD_INODE_ENTRY].TT[0];
	blk[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blk[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blk[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

	// its a directory; so the following should never happen
	if (it == 'F')
	{
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}

	// now lets try to see if a directory by the name already exists
	for (i = 0; i < 3; i++)
	{
		if (blk[i] == 0)
			continue; // 0 means pointing at nothing

		readSFS(blk[i], (char *)_directory_entries); // lets read a directory entry; notice the cast

		// so, we got four possible directory entries now
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0')
				continue; // means unused entry

			enod = stoi(_directory_entries[j].MMM, 3); // this is the inode that has more info about this entry

			if (_inode_table[enod].TT[0] == 'D')
			{ // entry is for a directory; can't cd into a file, right?
				if (strncmp(dname, _directory_entries[j].fname, 252) == 0)
				{			   // and it is the one we are looking for
					find = 1; // VOILA
					break;
				}
			}
		}
		if (find)
			break; // no need to search more
	}

	if (find)
	{
		CD_INODE_ENTRY = enod;						// just keep track of which inode entry in the table corresponds to this directory
		strncpy(current_working_directory, dname, 252); // can use it in the prompt
	}
	else
	{
		printf("%.252s: No such directory.\n", dname);
	}
}

/* creates a new directory called <dname> in the current directory if the */
/* name is not already taken and there is still space available */


void md(char *dname)
{
	char it;
	int blk[3];
	_directory_entry _directory_entries[4];

	int i, j;

	int remov_dblock = -1, remov_d = -1;
	int remov_i;

	// non-empty name
	if (strlen(dname) == 0)
	{
		printf("Usage: md <directory name>\n");
		return;
	}

	// do we have free inodes
	if (free_inode_entries == 0)
	{
		printf("Error: Inode table is full.\n");
		return;
	}

	// read inode entry for current directory
	// in SFS, an inode can point to three blk at the most
	it = _inode_table[CD_INODE_ENTRY].TT[0];
	blk[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blk[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blk[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

	// its a directory; so the following should never happen
	if (it == 'F')
	{
		printf("Fatal Error! Aborting.\n");
		exit(1);
	}

	// now lets try to see if the name already exists
	for (i = 0; i < 3; i++)
	{
		if (blk[i] == 0)
		{ // 0 means pointing at nothing
			if (remov_dblock == -1)
				remov_dblock = i; // we can later add a block if needed
			continue;
		}

		readSFS(blk[i], (char *)_directory_entries); // lets read a directory entry; notice the cast

		// so, we got four possible directory entries now
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0')
			{ // means unused entry
				if (remov_d == -1)
				{
					remov_d = j;
					remov_dblock = i;
				} // AAHA! lets keep a note of it, just in case we have to create the new directory
				continue;
			}

			if (strncmp(dname, _directory_entries[j].fname, 252) == 0)
			{ // compare with user given name
				printf("%.252s: Already exists.\n", dname);
				return;
			}
		}
	}
	// so directory name is new

	// if we did not find an empty directory entry and all three blk are in use; then no new directory can be made
	if (remov_d == -1 && remov_dblock == -1)
	{
		printf("Error: Maximum directory entries reached.\n");
		return;
	}
	else
	{ // otherwise
		if (remov_d == -1)
		{ // Great! didn't find an empty entry but not all three blk have been used
			remov_d = 0;

			if ((blk[remov_dblock] = getBlock()) == -1)
			{ // first get a new block using the block bitmap
				printf("Error: Disk is full.\n");
				return;
			}

			writeSFS(blk[remov_dblock], NULL); // write all zeros to the block (there may be junk from the past!)

			switch (remov_dblock)
			{ // update the inode entry of current dir to reflect that we are using a new block
			case 0:
				itos(_inode_table[CD_INODE_ENTRY].XX, blk[remov_dblock], 2);
				break;
			case 1:
				itos(_inode_table[CD_INODE_ENTRY].YY, blk[remov_dblock], 2);
				break;
			case 2:
				itos(_inode_table[CD_INODE_ENTRY].ZZ, blk[remov_dblock], 2);
				break;
			}
		}

		// NOTE: all error checkings have already been done at this point!!
		// time to put everything together

		remov_i = getInode(); // get an empty place in the inode table which will store info about blk for this new directory

		readSFS(blk[remov_dblock], (char *)_directory_entries);	 // read block of current directory where info on this new directory will be written
		_directory_entries[remov_d].F = '1';					 // remember we find which directory entry is unused; well, set it to used now
		strncpy(_directory_entries[remov_d].fname, dname, 252); // put the name in there
		itos(_directory_entries[remov_d].MMM, remov_i, 3); // and the index of the inode that will hold info inside this directory
		writeSFS(blk[remov_dblock], (char *)_directory_entries);	 // now write this block back to the disk

		strncpy(_inode_table[remov_i].TT, "DI", 2); // create the inode entry...first, its a directory, so DI
		strncpy(_inode_table[remov_i].XX, "00", 2); // directory is just created; so no blk assigned to it yet
		strncpy(_inode_table[remov_i].YY, "00", 2);
		strncpy(_inode_table[remov_i].ZZ, "00", 2);

		writeSFS(BLOCK_INODE_TABLE, (char *)_inode_table); // phew!! write the inode table back to the disk
	}
}

/****************************************************************************/
/* prints number of free blk in the disk and free inode entries in the inode table
/*
/****************************************************************************/

void stats()
{
	int blocks_free = BLB, inodes_free = INB;
	int i;

	for (i = 0; i < BLB; i++)
		blocks_free -= (_block_bitmap[i] - 48);
	for (i = 0; i < INB; i++)
		inodes_free -= (_inode_bitmap[i] - 48);

	printf("%d block%c free.\n", blocks_free, (blocks_free <= 1 ? 0 : 's'));
	printf("%d inode entr%s free.\n", inodes_free, (inodes_free <= 1 ? "y" : "ies"));
}

/****************************************************************************/
/* displays the contents of the file <fname> within the current directory if
/* it exists
/*
/****************************************************************************/

void display(char *fname)
{
	char it; // inode type
	int blk[3];
	_directory_entry _directory_entries[4];

	int i, j;
	int enod;

	char find = 0;
	char r_buffer[1024];  // r = read

	// take inode for directory
	// in SFS, We know Inode points to 3 
	it = _inode_table[CD_INODE_ENTRY].TT[0];
	blk[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
	blk[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
	blk[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

	// To check error in a directory
	if (it == 'F')
	{
		printf("Error, Aborting\n");
		exit(1);
	}

	// To check the file through name, whether it is there or not
	for (i = 0; i < 3; i++)
	{
		if (blk[i] == 0)
			continue; // 0 = pointing at none

		readSFS(blk[i], (char *)_directory_entries); // Read directory

		// Now we have 4 entries to look.
		for (j = 0; j < 4; j++)
		{
			if (_directory_entries[j].F == '0') // Not used entry
				continue; 

			enod = stoi(_directory_entries[j].MMM, 3); 

			if (_inode_table[enod].TT[0] == 'F')
			{ // Condition check for entry that we are looking for 
				if (strncmp(fname, _directory_entries[j].fname, 252) == 0)
				{			  
					find = 1; 
					break;
				}
			}
		}
		if (find)   // breaking to avoid more search
			break;
	}

	if (find)
	{ // displaying file 
		int f_block[3]; //f = file
		f_block[0] = stoi(_inode_table[enod].XX, 2);
		f_block[1] = stoi(_inode_table[enod].YY, 2);
		f_block[2] = stoi(_inode_table[enod].ZZ, 2);

		for (i = 0; i < 3; i++)
		{
			if (f_block[i] == 0)
				continue;

			readSFS(f_block[i], r_buffer);
			printf("%s", r_buffer);
		}
	}
	else
	{
		printf("%.252s: No file found.\n", fname);
	}
}

/****************************************************************************/
/*if the name is there and not taken, create a new file called <fname> in the present directory.
/****************************************************************************/

void create(char *fname)
{
    char it;
    int blk[3];
    _directory_entry _directory_entries[4];

    int i, j;

    int remov_dblock = -1, remov_d = -1;
    int remov_i; // i entry and d entry

    // check for empty name
    if (strlen(fname) == 0)
    {
        printf("Use: Create file name\n");
        return;
    }

    // look for free nodes
    if (free_inode_entries == 0)
    {
        printf("Error-> Inode table is filled.\n");
        return;
    }

	// same from display
    it = _inode_table[CD_INODE_ENTRY].TT[0];
    blk[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
    blk[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
    blk[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

    if (it == 'F')
    {
        printf("Error, Aborting.\n");
        exit(1);
    }

    // TO check the file through name, whether it is there or not
    for (i = 0; i < 3; i++)
    {
        if (blk[i] == 0)  // 0 = pointing at none
        {   
            if (remov_dblock == -1)
                remov_dblock = i; // Block can be added later
            continue;
        }

        readSFS(blk[i], (char *)_directory_entries); // Read directory

        // Now we have 4 entries to look
        for (j = 0; j < 4; j++)
        {
            if (_directory_entries[j].F == '0') // Not used entry
            { 
                if (remov_d == -1)
                {
                    remov_d = j;
                    remov_dblock = i;
                } 
                continue;
            }

            if (strncmp(fname, _directory_entries[j].fname, 252) == 0)
            { // To check if the fname exist or not
                printf("%.252s: Name found.\n", fname);
                return;
            }
        }
    }

/* For a new file, No new directory can be created if there isn't an empty directory entry and all three blocks are in use. */
    
    if (remov_d == -1 && remov_dblock == -1)
    {
        printf("Error-> Max. entries.\n");
        return;
    }
    else
    { 
        if (remov_d == -1)
        { // If we dont find empty but still blocks is available
            remov_d = 0;

            if ((blk[remov_dblock] = getBlock()) == -1)
            { // Obtaining block bitmap
                printf("Error-> Disk full.\n");
                return;
            }

            writeSFS(blk[remov_dblock], NULL); // Assigning 0 to all blks 

            switch (remov_dblock)
            { // change the present directory's inode entry to indicate that a new block is being used.

            case 0:
                itos(_inode_table[CD_INODE_ENTRY].XX, blk[remov_dblock], 2);
                break;
            case 1:
                itos(_inode_table[CD_INODE_ENTRY].YY, blk[remov_dblock], 2);
                break;
            case 2:
                itos(_inode_table[CD_INODE_ENTRY].ZZ, blk[remov_dblock], 2);
                break;
            }
        }

        remov_i = getInode(); // get a blank space in the inode table to store the blk information for this new file.


        readSFS(blk[remov_dblock], (char *)_directory_entries); // Reading block for present directory 
        _directory_entries[remov_d].F = '1'; // Directory changed to used
        strncpy(_directory_entries[remov_d].fname, fname, 252); // Appending name & indez of inode
        itos(_directory_entries[remov_d].MMM, remov_i, 3);
        writeSFS(blk[remov_dblock], (char *)_directory_entries); // Writing this blk to disk

        strncpy(_inode_table[remov_i].TT, "FI", 2); // Create the inode entry
        strncpy(_inode_table[remov_i].XX, "00", 2); // No blk is assigned yet.
        strncpy(_inode_table[remov_i].YY, "00", 2);
        strncpy(_inode_table[remov_i].ZZ, "00", 2);

        writeSFS(BLOCK_INODE_TABLE, (char *)_inode_table); // Writinginode table to disk

        // Reading user input until the user hits escape or reached more than 3072 char.
        char useri[3073];
        int useri_size = 0;
        char ch;

        printf("Enter content of file (use escape to finish):\n");

        while ((ch = getchar()) != 27 && useri_size < 3072)
        {
            useri[useri_size++] = ch;
        }

        useri[useri_size] = '\0';

        // Storing user input in file
        int user_info_blk[3];
        user_info_blk[0] = getBlock();
        user_info_blk[1] = getBlock();
        user_info_blk[2] = getBlock();

        for (i = 0; i < 3; i++)
        {
            if (user_info_blk[i] == -1)
            {
                printf("Error-> Disk full.\n");
                return;
            }

            int start_indexing = i * 1024;
            int end_indexing = start_indexing + 1024;
            char buffer[1024];

            strncpy(buffer, useri + start_indexing, 1024);

            writeSFS(user_info_blk[i], buffer);

            switch (i)
            {
            case 0:
                itos(_inode_table[remov_i].XX, user_info_blk[i], 2);
                break;
            case 1:
                itos(_inode_table[remov_i].YY, user_info_blk[i], 2);
                break;
            case 2:
                itos(_inode_table[remov_i].ZZ, user_info_blk[i], 2);
                break;
            }
        }

        // Updated inode table
        writeSFS(BLOCK_INODE_TABLE, (char *)_inode_table);

        printf("File '%s' created !\n", fname);
    }
}


/* removing file/dir with name from present directory */

void free_blk(int index)
{
    if (index > 3 && index <= BLOCK_MAX)
    {
        _block_bitmap[index] = '0';
        free_disk_blocks++;

        writeSFS(BLOCK_BLOCK_BITMAP, _block_bitmap);
    }
}

void rm(char *name)
{
    char it;
    int blk[3];
    _directory_entry _directory_entries[4];

    int i, j;

    int final_d = -1, final_dblock = -1;
    int final_i;

    // Checl non-empty name
    if (strlen(name) == 0)
    {
        printf("Use rm file/dir name>\n");
        return;
    }

    // take inode entry for directory

    it = _inode_table[CD_INODE_ENTRY].TT[0];
    blk[0] = stoi(_inode_table[CD_INODE_ENTRY].XX, 2);
    blk[1] = stoi(_inode_table[CD_INODE_ENTRY].YY, 2);
    blk[2] = stoi(_inode_table[CD_INODE_ENTRY].ZZ, 2);

    // To check error in directory
    if (it == 'F')
    {
        printf("Error, Aborting\n");
        exit(1);
    }

    // To check the file through name, whether it is there or not
    for (i = 0; i < 3; i++)
    {
        if (blk[i] == 0)
            continue; // 0 = pointing at none

        readSFS(blk[i], (char *)_directory_entries);

        // Now we have 4 entries to look.
        for (j = 0; j < 4; j++)
        {
            if (_directory_entries[j].F == '0') // Not used entry
                continue; 

            if (strncmp(name, _directory_entries[j].fname, 252) == 0)
            { // Check with name from user
                final_dblock = i;
                final_d = j;
                final_i = stoi(_directory_entries[j].MMM, 3);
                break;
            }
        }

        if (final_d != -1)
            break; // Check the final point, igonre the blk
    }

    if (final_d == -1)
    {
        printf("%.252s: No file or directory found.\n", name);
        return;
    }

    // If the final point is directory, continously remove its content from table
    if (_inode_table[final_i].TT[0] == 'D')
    {
        for (i = 0; i < 3; i++)
        {
            int blk_free = stoi(_inode_table[final_i].XX, 2 + i);

            if (blk_free != 0)
                free_blk(blk_free);
        }
    }

    // To indicate that the directory entry is not needed, change it.

    _directory_entries[final_d].F = '0';
    writeSFS(blk[final_dblock], (char *)_directory_entries);

    // To indicate that the inode entry is not needed, change it.
    _inode_table[final_i].TT[0] = 'U'; // U = not used
    writeSFS(BLOCK_INODE_TABLE, (char *)_inode_table);

    // Return the block to the system if the deleted file or directory is the final entry in it.

    if (_directory_entries[0].F == '0' && _directory_entries[1].F == '0' && _directory_entries[2].F == '0' && _directory_entries[3].F == '0')
    {
        free_blk(blk[final_dblock]);
        switch (final_dblock)
        {
        case 0:
            _inode_table[CD_INODE_ENTRY].XX[0] = '0';
            _inode_table[CD_INODE_ENTRY].XX[1] = '0';
            break;
        case 1:
            _inode_table[CD_INODE_ENTRY].YY[0] = '0';
            _inode_table[CD_INODE_ENTRY].YY[1] = '0';
            break;
        case 2:
            _inode_table[CD_INODE_ENTRY].ZZ[0] = '0';
            _inode_table[CD_INODE_ENTRY].ZZ[1] = '0';
            break;
        }
        writeSFS(BLOCK_INODE_TABLE, (char *)_inode_table);
    }

    printf("%.252s: Removed.\n", name);
}



int main()
{
	char cmdline[1024];
	int num_tokens = 0;
	char tokens[2][256];
	int i = 0;
	char *p;

	mountSFS();

	while (1)
	{
		num_tokens = 0;
		i = 0;
		printPrompt();

		if (fgets(cmdline, 1024, stdin) == NULL)
		{
			printf("\n");
			break;
		}

		*strchr(cmdline, '\n') = '\0';

		p = cmdline;
		while (1 == sscanf(p, "%s", tokens[i]))
		{
			p = strstr(p, tokens[i]) + strlen(tokens[i]);
			i++;
		}
		num_tokens = i;

		if (num_tokens == 0)
			continue;

		if (num_tokens == 1)
		{
			if (strcmp(tokens[0], "ls") == 0)
				ls();
			else if (strcmp(tokens[0], "exit") == 0)
				break;
			else if (strcmp(tokens[0], "stats") == 0)
				stats();
			else if (strcmp(tokens[0], "rd") == 0)
				rd();
			else
				continue;
		}

		if (num_tokens == 2)
		{
			if (strcmp(tokens[0], "md") == 0)
				md(tokens[1]);
			if (strcmp(tokens[0], "cd") == 0)
				cd(tokens[1]);
			if (strcmp(tokens[0], "display") == 0)
				display(tokens[1]);
			if (strcmp(tokens[0], "create") == 0)
				create(tokens[1]);
			if (strcmp(tokens[0], "rm") == 0)
				rm(tokens[1]);
		}
	}

	return 0;
}
