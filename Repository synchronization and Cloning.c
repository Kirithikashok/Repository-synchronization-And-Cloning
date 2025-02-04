#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

// Function to recursively synchronize repositories
void syncRepositories(const char *source, const char *destination);

// Function to clone a repository
void cloneRepository(const char *source, const char *destination);

// Function to pull a repository
void pullRepository(const char *destination);

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <source_repository> <destination_repository>\n", argv[0]);
    return 1;
  }

  const char *sourceRepository = argv[1];
  const char *destinationRepository = argv[2];

  // Check if the source repository exists
  if (access(sourceRepository, F_OK) == -1) {
    fprintf(stderr, "Source repository does not exist: %s\n", sourceRepository);
    return 1;
  }

  // Check if the destination directory exists
  if (access(destinationRepository, F_OK) == -1) {
    // Create the destination directory
    mkdir(destinationRepository, 0777);
  }

  // Synchronize the repositories
  syncRepositories(sourceRepository, destinationRepository);

  // Print a success message
  printf("Repository synchronization complete.\n");

  return 0;
}

void syncRepositories(const char *source, const char *destination) {
  // Check if the source repository is a git repository
  if (!isGitRepository(source)) {
    fprintf(stderr, "Source repository is not a git repository: %s\n", source);
    return;
  }

  // Check if the destination repository is a git repository
  if (!isGitRepository(destination)) {
    // Clone the source repository to the destination directory
    cloneRepository(source, destination);
  } else {
    // Pull the destination repository
    pullRepository(destination);
  }
}

void cloneRepository(const char *source, const char *destination) {
  // Clone the source repository to the destination directory
  int status = system("git clone %s %s", source, destination);
  if (status != 0) {
    fprintf(stderr, "Failed to clone repository: %s\n", strerror(errno));
  }
}

void pullRepository(const char *destination) {
  // Pull the destination repository
  int status = system("git pull", destination);
  if (status != 0) {
    fprintf(stderr, "Failed to pull repository: %s\n", strerror(errno));
  }
}

// Function to check if a directory is a git repository
bool isGitRepository(const char *path) {
  // Check if the .git directory exists
  if (access(path, F_OK) == -1) {
    return false;
  }

  // Check if the .git directory is a directory
  struct stat st;
  stat(path, &st);
  if (!S_ISDIR(st.st_mode)) {
    return false;
  }

  return true;
}S
