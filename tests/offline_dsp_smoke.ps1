$ErrorActionPreference = 'Stop'
$sr = 48000.0
$centers = @(@(700,1100,2450,3300),@(500,1900,2600,3400),@(300,2200,3000,3600),@(500,900,2400,3300),@(400,800,2200,3000))
$shifts = @((0.78 + 0.48 * 0.52), (0.82 + 0.48 * 0.44), (0.92 + 0.48 * 0.3), (1 + 0.48 * 0.18))
for ($vowel = 0; $vowel -lt 5; $vowel++) {
    $hz = 440.0 * [Math]::Pow(2.0, (60 - 69) / 12.0) * (1 + 0.48 * 0.06)
    $sumSquares = 0.0; $peak = 0.0; $count = 256
    for ($i = 0; $i -lt $count; $i++) {
        $env = 1.0
        $sample = 0.0; $phase = 2.0 * [Math]::PI * $hz * $i / $sr
        $maxH = [Math]::Min(16, [int]($sr / (2.2 * $hz)))
        for ($h = 1; $h -le $maxH; $h++) {
            $freq = $hz * $h; $weight = 0.0
            for ($j = 0; $j -lt 4; $j++) {
                $center = $centers[$vowel][$j] * $shifts[$j]; $width = $center * (0.20 + 0.13 * 0.58)
                $d = ($freq - $center) / $width
                $weight += $(if ($j -eq 0) { 1.0 } else { 0.74 }) * [Math]::Exp(-0.5 * $d * $d)
            }
            $sample += [Math]::Sin($phase * $h) * $weight * [Math]::Pow($h, -(1.08 + 0.58 * 0.65))
        }
        $sample = [Math]::Tanh($sample * (1.45 + 0.58) * 0.12) * $env * 0.24
        if ([double]::IsNaN($sample) -or [double]::IsInfinity($sample)) { throw "Non-finite output in vowel $vowel" }
        $peak = [Math]::Max($peak, [Math]::Abs($sample)); $sumSquares += $sample * $sample
    }
    $rms = [Math]::Sqrt($sumSquares / $count)
    if ($peak -le 0.005 -or $peak -ge 0.24 -or $rms -le 0.0001 -or $rms -ge 0.1) { throw "Level out of bounds for vowel $vowel (peak=$peak RMS=$rms)" }
    Write-Output ("Vowel {0}: {1} samples, peak={2:F4}, RMS={3:F4}" -f $vowel,$count,$peak,$rms)
}
Write-Output 'PASS: reference DSP output is finite, non-silent, and level-bounded.'
