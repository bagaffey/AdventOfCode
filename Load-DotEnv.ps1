# Run this to allow this script to be executed, bypassing the default Windows security policy which prevents PS scripts
# from being executed.
Set-ExecutionPolicy -Scope Process Bypass

function Import-DotEnv {
    param([string]$Path = ".env")

    if (-Not (Test-Path $Path)) {
        Write-Error "File not found: $Path"
        return
    }

    Get-Content $Path | ForEach-Object {
        if ($_ -match '^\s*([^#=]+?)\s*=\s*(.*)\s*$') {
            $name = $matches[1].Trim()
            $value = $matches[2].Trim('"').Trim("'").Trim()
            [System.Environment]::SetEnvironmentVariable($name, $value, "Process")
        }
    }
}

Import-DotEnv(".env")