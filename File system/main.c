#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

char *strfcat(char *str1, char *str2)
{
	char *tmp = strdup(str1);

	strcpy(str1, str2);	//Put str2 or anyother string
						//that you want at the begining
	strcat(str1, tmp);	//concatenate previous str1

	free(tmp);	//free the memory
	tmp = NULL;

	return str1;
}

// Root folder initialization
Directory *initFS(void)
{
	Directory *root = malloc(sizeof(Directory));

	root->name = malloc(2);
	strcpy(root->name, "/\0");
	root->parentDir = NULL;
	root->Files = NULL;
	root->Directories = NULL;
	root->lenF = 0;
	root->lenD = 0;
	return root;
}

// Allocates memory for new node and returns a pointer to it
Node *new_node(void *data)
{
	Node *result = malloc(sizeof(Node));

	result->data = data;
	result->next = NULL;
	return result;
}

// New file function for "touch" command
File *new_file(char *name, int size, char *data, Directory *dir)
{
	File *f = malloc(sizeof(File));

	f->name = name;
	f->data = data;
	f->size = size;
	dir->lenF++;
	f->dir = dir;
	return f;
}

Directory *new_dir(char *name, Directory *dir)
{
	Directory *d = malloc(sizeof(Directory));

	d->Directories = NULL;
	d->Files = NULL;
	d->name = name;
	d->lenF = 0;
	d->lenD = 0;
	dir->lenD++;
	d->parentDir = dir;
	return d;
}

static inline void indent(int n)
{
	for (int i = 0; i < n; i++)
		printf("    ");
}

void tree(Directory dir, int t)
{
	int k = 0;

	printf("%s\n", dir.name);
	if (dir.Files != NULL) {
		++t;
		k = 1;
	}
	while (dir.Files != NULL) {
		indent(t);
		printf("%s\n", ((File *) dir.Files->data)->name);
		dir.Files = dir.Files->next;
	}
	if (!k)
		++t;
	while (dir.Directories != NULL) {
		indent(t);
		tree(*(Directory *) dir.Directories->data, t);
		dir.Directories = dir.Directories->next;
	}
}

void rm(Node *fileList, char *fileName, Directory *current_dir)
{
	if (strcmp(((File *) fileList->data)->name, fileName) == 0) {
		free(((File *) fileList->data)->name);
		free(((File *) fileList->data)->data);
		free((File *) fileList->data);
		if (fileList->next != NULL) {
			current_dir->Files = fileList->next;
			free(fileList);
		} else
			free(fileList);
		current_dir->lenF--;
		return;
	}

	while (fileList->next != NULL) {
		if (strcmp(((File *) fileList->next->data)->name,
				fileName) == 0) {
			free(((File *) fileList->next->data)->name);
			free(((File *) fileList->next->data)->data);
			free((File *) fileList->next->data);
			if (fileList->next->next != NULL) {
				Node *del = fileList->next;

				fileList->next = fileList->next->next;
				del->next = NULL;
				free(del);
			} else {
				Node *del = fileList->next;

				fileList->next = NULL;
				del->next = NULL;
				free(del);
			}
			current_dir->lenF--;
			return;
		}
		fileList = fileList->next;
	}
	printf("Cannot remove '%s': No such file!\n", fileName);
}

void rmdir(Directory *current_dir)
{
	Node *dirList = current_dir->Directories;

	while (current_dir->Files != NULL) {
		Node *fileList;

		fileList = current_dir->Files;
		while (fileList != NULL) {
			free(((File *) fileList->data)->name);
			free(((File *) fileList->data)->data);
			free((File *) fileList->data);
			Node *del = fileList;

			fileList = fileList->next;
			del->next = NULL;
			free(del);
		}
		current_dir->Files = NULL;
	}
	while (dirList != NULL) {
		rmdir((Directory *) dirList->data);
		free(((Directory *) dirList->data)->name);
		free((Directory *) dirList->data);
		Node *del = dirList;

		dirList = dirList->next;
		del->next = NULL;
		free(del);
	}
}

void searchAndDelete(Node *dirList,
	Directory *current_dir, char *runner)
{
	while (dirList->next != NULL) {
		if (strcmp(((Directory *) dirList->next->data)->name,
				runner) == 0) {
			rmdir((Directory *) dirList->next->data);
			free(((Directory *) dirList->next->data)->name);
			free((Directory *) dirList->next->data);
			Node **prevP, *aux;

			prevP = &dirList;
			aux = dirList->next;
			(*prevP)->next = (*prevP)->next->next;
			aux->next = NULL;
			free(aux);
			current_dir->lenD--;
			return;
		}
		dirList = dirList->next;
	}
	if (dirList->next == NULL) {
		printf("Cannot remove '%s': No such directory!\n", runner);
		return;
	}
}

void find(Directory *current_dir, char *runner, int min_size,
	int max_size, int max_depth, int depth)
{
	Node *fileList;

	fileList = current_dir->Files;
	while (fileList != NULL) {
		if (strstr(((File *) fileList->data)->data, runner) != NULL)
			if (((File *) fileList->data)->size >= min_size &&
				((File *) fileList->data)->size <= max_size)
				printf("%s ", ((File *) fileList->data)->name);

		fileList = fileList->next;
	}
	Node *dirList;

	dirList = current_dir->Directories;
	while (dirList != NULL && depth < max_depth) {
		find((Directory *) dirList->data, runner, min_size,
			max_size, max_depth, ++depth);
		depth--;
		dirList = dirList->next;
	}
}

int main(void)
{
	char *line = NULL;
	size_t len = 0;
	Directory *root = NULL;	//directorul radacina
	Directory *current_dir = NULL;	//directorul curent
	File *tmpFile;	//fisier temporar
	Node *tmpNode;	//nod temporar
	Directory *tmpDir;	//director temporar

	//Command reader loop
	while ((getline(&line, &len, stdin)) != -1) {
		if (!strcmp(line, "create fs\n")) {
			root = initFS();
			current_dir = root;
		}

		if (strstr(line, "touch") != NULL) {
			char *file_name = NULL, *file_data = NULL, *runner = NULL;

			runner = strtok(line, " \n");
			runner = strtok(NULL, " \n");
			file_name = strdup(runner);
			runner = strtok(NULL, " \n");
			file_data = strdup(runner);
			int size = strlen(file_data);

			tmpFile = new_file(file_name, size, file_data, current_dir);

			tmpNode = new_node(tmpFile);

			if (current_dir->Files == NULL)
				current_dir->Files = tmpNode;
			else {
				Node *iter, *parent;
				int k = 0, j = current_dir->lenF - 1;

				iter = current_dir->Files;
				parent = iter;
				while ((j > 0) && strcmp(((File *) iter->data)->name,
					file_name) < 0) {
					if (iter->next == NULL) {
						iter->next = tmpNode;
						k = 1;
					}
					j--;
					parent = iter;
					iter = iter->next;
				}
				if (!k && parent->next != NULL &&
					strcmp(((File *) parent->data)->name, file_name) < 0) {
					tmpNode->next = parent->next;
					parent->next = tmpNode;
					k = 1;
				}
				if (!k) {
					tmpNode->next = current_dir->Files;
					current_dir->Files = tmpNode;
				}
			}
		}

		if (strstr(line, "mkdir") != NULL) {
			char *dir_name = NULL, *runner = NULL;

			runner = strtok(line, " \n");
			runner = strtok(NULL, " \n");
			dir_name = strdup(runner);
			tmpDir = new_dir(dir_name, current_dir);
			tmpNode = new_node(tmpDir);
			if (current_dir->Directories == NULL)
				current_dir->Directories = tmpNode;
			else {
				Node *iter, *parent;
				int k = 0, j = current_dir->lenD - 1;

				iter = current_dir->Directories;
				parent = iter;
				while ((j > 0) &&
					strcmp(((Directory *) iter->data)->name, dir_name) < 0) {
					if (iter->next == NULL) {
						iter->next = tmpNode;
						k = 1;
					}
					j--;
					parent = iter;
					iter = iter->next;
				}
				if (!k && parent->next != NULL &&
					strcmp(((Directory *) parent->data)->name, dir_name) < 0) {
					tmpNode->next = parent->next;
					parent->next = tmpNode;
					k = 1;
				}
				if (!k) {
					tmpNode->next = current_dir->Directories;
					current_dir->Directories = tmpNode;
				}
			}
		}

		if (strstr(line, "ls") != NULL) {
			Directory lsDir = *current_dir;

			for (int i = 0; i < lsDir.lenF; ++i) {
				printf("%s ", ((File *) lsDir.Files->data)->name);
				lsDir.Files = lsDir.Files->next;
			}
			lsDir = *current_dir;

			for (int i = 0; i < lsDir.lenD; ++i) {
				printf("%s ", ((Directory *) lsDir.Directories->data)->name);
				lsDir.Directories = lsDir.Directories->next;
			}
			printf("\n");
		}

		if (strstr(line, "pwd") != NULL) {
			char path[100] = "";
			Directory pwd;

			pwd = *current_dir;
			while (pwd.parentDir != NULL) {
				if (path[0] == '\0')
					strfcat(path, pwd.name);
				else {
					strfcat(path, "/");
					strfcat(path, pwd.name);
				}
				pwd = *pwd.parentDir;
			}
			printf("/%s\n", path);
		}

		if (strstr(line, "cd") != NULL) {
			char *runner = NULL;

			runner = strtok(line, " \n");
			runner = strtok(NULL, " \n");
			Node *cd;

			cd = current_dir->Directories;
			if (strcmp(runner, "..") != 0) {
				if (current_dir->lenD) {
					while (strcmp(runner,
							((Directory *) cd->data)->name) != 0
							&& cd->next != NULL)
						cd = cd->next;
					if (strcmp(runner,
							((Directory *) cd->data)->name) == 0)
						current_dir = (Directory *) cd->data;
					else
						printf("Cannot move to '%s': No such directory!\n",
							runner);
				} else
					printf("Cannot move to '%s': No such directory!\n",
						runner);
			} else if (!strcmp(runner, "..") &&
					current_dir->parentDir != NULL)
				current_dir = current_dir->parentDir;
		}

		if (strstr(line, "tree") != NULL)
			tree(*current_dir, 0);

		if (strstr(line, "rmdir") != NULL) {
			char *runner = NULL;

			runner = strtok(line, " \n");
			runner = strtok(NULL, " \n");
			Node *dirList;

			dirList = current_dir->Directories;
			if (strcmp(((Directory *) dirList->data)->name,
					runner) == 0) {
				rmdir((Directory *) dirList->data);
				free(((Directory *) dirList->data)->name);
				free((Directory *) dirList->data);
				Node *del = current_dir->Directories;

				current_dir->Directories =
					current_dir->Directories->next;
				del->next = NULL;
				free(del);
				current_dir->lenD--;
			} else
				searchAndDelete(current_dir->Directories,
					current_dir, runner);
		} else if (strstr(line, "rm") != NULL) {
			char *runner = NULL;

			runner = strtok(line, " \n");
			runner = strtok(NULL, " \n");
			if (current_dir->Files != NULL)
				rm(current_dir->Files, runner, current_dir);
			else
				printf("Cannot remove '%s': No such file!\n", runner);
		}

		if (strstr(line, "find") != NULL) {
			int max_depth, min_size, max_size;
			char *runner = NULL;

			runner = strtok(line, " \n");
			runner = strtok(NULL, " \n");
			max_depth = atoi(runner);
			runner = strtok(NULL, " \n");
			min_size = atoi(runner);
			runner = strtok(NULL, " \n");
			max_size = atoi(runner);
			runner = strtok(NULL, " \n");
			find(current_dir, runner, min_size, max_size, max_depth, 0);
			printf("\n");
		}

		if (strstr(line, "delete fs") != NULL) {
			free(root->name);
			free(root);
		}
	}
	if (line)
		free(line);

	return 0;
}