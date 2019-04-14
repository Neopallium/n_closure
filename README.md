# n_closure.h

A header-only closure library for C.

#### Build Status:

|Platform|Build Status|
|--------|------------|
|Linux/OSX|[![Build Status](https://travis-ci.org/Neopallium/n_closure.svg?branch=master)](https://travis-ci.org/Neopallium/n_closure)|
|Windows (Win32/Win64)|[![Build status](https://ci.appveyor.com/api/projects/status/66ix26hl14gk0k5m?svg=true)](https://ci.appveyor.com/project/Neopallium/n-closure)|

### TODO

- Find a safe way to insert a `void *userdata` parameter.  Win64 ABI makes this difficult because of how it handles the first 4 parameters.  Will need some assembly on Win64.

