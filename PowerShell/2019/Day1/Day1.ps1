<#
.SYNOPSIS
    Advent of Code 2019 Day 1 Fuel Solution in PowerShell

.DESCRIPTION
    - If an input path is provided, reads masses from that file.
    - Otherwise, uses a cached file in %TEMP% (2019-12-01-1.dat).
    If not cached, downloads from AoC using AOC_SESSION cookie and caches it.
#>

[CmdletBinding()]
param(
    [Parameter(Position = 0)]
    [string] $InputPath
)

Set-StrictMode -Version Latest
$ErrorActionPreference = 'Stop'

function Get-MassesFromFile {
    param([Parameter(Mandatory)][string] $Path)

    if (-not (Test-Path -LiteralPath $Path)) {
        throw "Input file not found: $Path"
    }

    Get-Content -LiteralPath $Path -Encoding UTF8 |
        ForEach-Object { $_.Trim() } |
        Where-Object { $_ -match '^\d+$' } |
        ForEach-Object { [int]$_ }
}

function Get-MassesFromAOC {
    param([Parameter(Mandatory)][string] $CachePath)

    $Session = $env:AOC_SESSION
    if ([string]::IsNullOrWhiteSpace($Session)) {
        throw "AOC_SESSION environment variable is not set. Set it and run again."
    }

    $Url = 'https://adventofcode.com/2019/day/1/input'
    $Headers = @{
        'Cookie'    = "session=$Session"
        'User-Agent' = 'aoc-fetcher/1.0'
    }

    # Download Input
    $Resp = Invoke-WebRequest -Uri $url -Headers $Headers
    $Content = $Resp.Content

    $Normalized = ($Content.TrimEnd("`r","`n") + "`n")

    # Cache the data
    [void] (New-Item -ItemType File -Path $CachePath -Force)
    Set-Content -LiteralPath $CachePath -Value $Normalized -Encoding UTF8

    $Normalized -split "`n" |
        ForEach-Object { $_.Trim() } |
        Where-Object { $_ -match '^\d+$' } |
        ForEach-Object { [int]$_ }
}
