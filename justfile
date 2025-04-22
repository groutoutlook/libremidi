shebang := if os() == 'windows' { 'pwsh.exe' } else { '/usr/bin/env pwsh' }
set shell := ["pwsh", "-c"]
set windows-shell := ["pwsh.exe", "-NoLogo", "-Command"]
set dotenv-load := true
# set dotenv-filename	:= ".env"
# set dotenv-required := true
# INFO: if you want to edit the justfile use js -e.

help:
    @just --list -f "{{home_directory()}}/justfile"

default_arg := 'TODO:'
alias td := todo
todo todo=default_arg:
    rg {{todo}} -g '!justfile' -g "!third_party" 

hello:
    @Write-Host "Hello, world!" -ForegroundColor Yellow

placeholder:
    #!{{ shebang }}
    Write-Host "Havent written build task for this repo." -ForegroundColor Red
    if($env:pwsh_env) {Write-Host "$env:pwsh_env"}
    else {Write-Host "Apparently no .env as well" -ForegroundColor Yellow}

# INFO: basic `run` recipe.
alias r := run
default_binraies_path := 'out/build/x64-debug'
default_args := 'midiout.exe'
run args=default_args:
    {{default_binraies_path}}/{{args}}


alias b := build
build:
    cmake -S. -Bbuild

alias t := soundtest
default_midi := "out\\build\\x64-debug\\x.mid"
soundtest args=default_midi:
    vlc {{default_midi}}


format: 
    # could be something as `biome format --write`
