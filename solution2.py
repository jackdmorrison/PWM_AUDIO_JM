wav_position=0
pulseLength = 86
wavelength = 172
tan_theta=1/86
n_tan_theta = wavelength-pulseLength
while wav_position < wavelength:
    if (wav_position <= (pulseLength) ) : 
        print(255*(((wav_position)/86)))
        

    elif(wav_position<(wavelength) - 1):
        print((1-((wav_position-pulseLength)/n_tan_theta))*255)
    wav_position+=1    
    