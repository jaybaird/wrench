  # wrench — a silly little Wren "compiler."
  
  This is `wrench`, a super simple [Wren](http://wren.io) compiler used to detect syntax errors before I run my host applications. Most of the code is borrowed from [`wren-cli`](https://wren.io/cli/) and adapted here for my simple use-case.

  Hook it up to Visual Studio Code's [`RunOnSave` extension](https://marketplace.visualstudio.com/items?itemName=emeraldwalk.RunOnSave) to check your
  wren code for compile errors, e.g.

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
