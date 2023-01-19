/*
  Super simple wren "compiler." Most of the code borrowed from `wren-cli`
  and adapted here for my simple use-case.

  Hook it up to Visual Studio Code's `RunOnSave` extension to check your
  wren code for errors, e.g.

  ```
  "emeraldwalk.runonsave": {

        "autoClearConsole": true,
        "commands": [
            {
                "match": "\\.wren$",
                "isAsync": true,
                "cmd": "${workspaceFolder}\\tools\\bin\\wrenc.exe ${file}"
            }
        ]
    }
  ```
*/

#include <stdio.h>
#include <string.h>

#include "wren.h"

#define WRENC_VERSION_STRING "1.0"

#ifndef EX_NOINPUT
#define EX_NOINPUT 66
#endif
#ifndef EX_IOERR
#define EX_IOERR 74
#endif

static char *
readFile(const char *path)
{
  FILE *file;
  fopen_s(&file, path, "rb");
  if (file == NULL)
    return NULL;

  // Find out how big the file is.
  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  // Allocate a buffer for it.
  char *buffer = (char *)malloc(fileSize + 1);
  if (buffer == NULL)
  {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(EX_IOERR);
  }

  // Read the entire file.
  size_t bytesRead = fread(buffer, 1, fileSize, file);
  if (bytesRead < fileSize)
  {
    fprintf(stderr, "Could not read file \"%s\".\n", path);
    exit(EX_IOERR);
  }

  // Terminate the string.
  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

static void reportError(WrenVM *vm, WrenErrorType type,
                        const char *module, int line, const char *message)
{
  switch (type)
  {
  case WREN_ERROR_COMPILE:
    fprintf(stderr, "[%s line %d] %s\n", module, line, message);
    break;
  default:
    break;
  }
}

static const char* resolveModule(WrenVM* vm, const char* importer,
                                 const char* module)
{
  return "";
}

int main(int argc, char* argv[])
{
  if (argc == 1 || (argc == 2 && strcmp(argv[1], "--help") == 0))
  {
    printf("Usage: wrenc [file]\n");
    printf("\n");
    printf("Optional arguments:\n");
    printf("  --help     Show command line usage\n");
    printf("  --version  Show version\n");
    return 0;
  }

  if (argc == 2 && strcmp(argv[1], "--version") == 0)
  {
    printf("wrenc %s; wren %s\n", WRENC_VERSION_STRING, WREN_VERSION_STRING);
    return 0;
  }

  char *source = readFile(argv[1]);
  if (source == NULL)
  {
    fprintf(stderr, "Could not find file \"%s\".\n", argv[1]);
    exit(EX_NOINPUT);
  }

  WrenConfiguration config;
  wrenInitConfiguration(&config);

  config.resolveModuleFn = resolveModule;
  config.errorFn = reportError;

  WrenVM* vm = wrenNewVM(&config);
  WrenInterpretResult result = wrenInterpret(vm, argv[1], source);
  wrenFreeVM(vm);

  return result;
}
