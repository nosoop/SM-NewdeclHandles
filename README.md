# Methodmap wrappers for Extension Handles
Methodmaps for a small set of extensions.

## Usage

The repository's intended to be used as a git submodule, so you can do the following:

```sh
git submodule add https://github.com/nosoop/newdecl-handles scripting/include/newdecl-handles
```

Though you can download the archive and extract it to a subdirectory under your includes.

You can then import it as follows:

```
#include <newdecl-handles/...>
```

Note that you also need the existing extension's include file; these includes are just fancy wrappers around the existing ones.
These also enforce transitional syntax when included.
