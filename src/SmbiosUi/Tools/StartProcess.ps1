function Start-Process-And-Wait([String[][]] $Pairs, [Boolean]$Inline = $false, [String]$WorkingDirectory = "", [Boolean]$ThrowOnCrash = $true) {
    $processes = New-Object System.Diagnostics.Process[] $Pairs.Length
    for ($i = 0; $i -lt $Pairs.Length; $i++) {
        Write-Host "    Running: $($Pairs[$i][0]) $($Pairs[$i][1])" -ForegroundColor DarkGray

        $arguments = @{};
        if ($Pairs[$i][1] -ne "") {
            $arguments.Add("ArgumentList", $Pairs[$i][1])
        }
        $arguments.Add("PassThru", $true)
        $arguments.Add("NoNewWindow", $Inline)
        if ($WorkingDirectory -ne "") {
            $arguments.Add("WorkingDirectory", $WorkingDirectory)
        }

        $processes[$i] = Start-Process $Pairs[$i][0] @arguments
    }

    $failed = $false
    for ($i = 0; $i -lt $Pairs.Length; $i++) {
        $process = $processes[$i]
        $process_handle = $process.Handle
        $process.WaitForExit()
        $process.Close()
    }

    [Console]::ResetColor()

    if ($failed) {
        throw "One or more processes crash"
    }
}