index[2:0] := 0
min[15:0] := a[15:0]
FOR j := 0 to 7
	i := j*16
	IF a[i+15:i] < min[15:0]
		index[2:0] := j
		min[15:0] := a[i+15:i]
	FI
ENDFOR
dst[15:0] := min[15:0]
dst[18:16] := index[2:0]
dst[127:19] := 0