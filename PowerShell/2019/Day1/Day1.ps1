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
    [OutputType([int[]])]
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
    [OutputType([int[]])]
    param([Parameter(Mandatory)][string] $CachePath)

    $Session = $env:AOC_SESSION
    if ([string]::IsNullOrWhiteSpace($Session)) {
        throw "AOC_SESSION environment variable is not set. Set it and run again."
    }

    $WebSession = New-Object Microsoft.PowerShell.Commands.WebRequestSession
    $Cookie = New-Object System.Net.Cookie('session', $Session, '/', '.adventofcode.com')
    $WebSession.Cookies.Add($Cookie)
    
    $Url = 'https://adventofcode.com/2019/day/1/input'
    $Headers = @{
        'User-Agent' = 'aoc-fetcher/1.0'
        'Accept'    = 'text/plain'
    }

    # Download Input
    try {
        $Resp = Invoke-WebRequest -Uri $Url -Headers $Headers -WebSession $WebSession -ErrorAction Stop
    }
    catch {
        $status = if ($_.Exception.Response) { $_.Exception.Response.StatusCode.value__ } else { 'n/a' }
        $reason = if ($_.Exception.Response) { $_.Exception.Response.ReasonPhrase } else { $_.Exception.Message }
        throw "Download failed. HTTP $status $reason. Check AOC_SESSION, UA, and connectivity."
    }

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

function Get-MassesFromCacheOrRedownload {
    [CmdletBinding()]
    [OutputType([int[]])]
    param()
    $CachePath = Join-Path $env:TEMP '2019-12-01-1.dat'
    if (Test-Path -LiteralPath $CachePath) {
        Get-Content -LiteralPath $CachePath -Encoding UTF8 |
            ForEach-Object { $_.Trim() } |
            Where-Object { $_ } |
            ForEach-Object { [int]$_ }
    }
    else {
        Get-MassesFromAOC -CachePath $CachePath
    }
}

function Get-Fuel {
    [OutputType([int])]
    param([Parameter(Mandatory)][int] $Mass)
    [math]::Floor( $Mass / 3 ) - 2
}

function Get-RealFuel {
    [OutputType([int])]
    param([Parameter(Mandatory)][int] $Mass)
    $Fuel = [math]::Floor( $Mass / 3 ) - 2
    if ($Fuel -le 0) {
        return (0)
    }
    return ($Fuel + (Get-RealFuel -Mass $Fuel))
}

try {
    if ($InputPath) {
        $Masses = Get-MassesFromFile -Path $InputPath
    }
    else {
        $Masses = Get-MassesFromCacheOrRedownload
    }

    $TotalModulesFuel = ($Masses | ForEach-Object { Get-Fuel -Mass $_ } | Measure-Object -Sum ).Sum
    Write-Host ""
    Write-Host "(Part 1) - Transport masses fuel required: $TotalModulesFuel"

    $TotalTripFuel = ($Masses | ForEach-Object { Get-RealFuel -Mass $_ } | Measure-Object -Sum).Sum
    Write-Host ""
    Write-Host "(Part 2) - Total Fuel for trip required: $TotalTripFuel"
}
catch {
    Write-Host "Error: $($_.Exception.Message)"
    exit 1
}