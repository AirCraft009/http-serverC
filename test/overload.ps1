param(
    [string]$TargetHost = "localhost",
    [int]$Port = 8080,
    [int]$Concurrency = 50,   # max parallel ncat processes
    [int]$Total = 500,          # total connections to start; 0 = infinite
    [int]$DelayMs = 10        # delay between starting processes (ms)
)

# Ensure request file exists
$reqFile = Join-Path (Split-Path -Parent $MyInvocation.MyCommand.Definition) "request.txt"
if (-not (Test-Path $reqFile)) {
    Write-Error "Request file '$reqFile' not found. Create it with real CRLFs."
    exit 1
}

# Ensure ncat is available
$nc = Get-Command ncat -ErrorAction SilentlyContinue
if (-not $nc) {
    Write-Error "ncat not found in PATH. Ensure ncat is installed and available."
    exit 1
}

Write-Host "Using request file: $reqFile"
Write-Host "ncat path: $($nc.Source)"
Write-Host "Host: $TargetHost  Port: $Port  Concurrency: $Concurrency  Total: $Total"

$running = @()
$started = 0

function Start-Ncat {
    param($TargetHost,$port,$req)
    # Use Start-Process with RedirectStandardInput so ncat reads the file
    $args = "$TargetHost $port"
    return Start-Process -FilePath $nc.Source -ArgumentList $args -RedirectStandardInput $req -NoNewWindow -PassThru
}

try {
    while (($Total -eq 0) -or ($started -lt $Total)) {
        # remove exited processes
        $running = $running | Where-Object { -not $_.HasExited }

        if ($running.Count -ge $Concurrency) {
            Start-Sleep -Milliseconds 100
            continue
        }

        $proc = Start-Ncat -TargetHost $TargetHost -port $Port -req $reqFile
        if ($proc) {
            $started++
        } else {
            Write-Warning "Failed to start ncat process"
        }

        if (($started % 50) -eq 0) { Write-Host "Started: $started  Running: $($running.Count)" }

        Start-Sleep -Milliseconds $DelayMs
    }

    Write-Host "Launched $started processes. Waiting for remaining to exit..."
    foreach ($p in $running) {
        try { $p.WaitForExit() } catch {}
    }
    Write-Host "Done."
}
finally {
    # Kill lingering ncat children if interrupted
    $running = $running | Where-Object { -not $_.HasExited }
    foreach ($p in $running) {
        try { $p.Kill() } catch {}
    }
}
