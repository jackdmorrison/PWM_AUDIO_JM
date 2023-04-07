import math
class wave:
    def __init__(self,frequency,amplitude,samplerate,wrap):
        self.frequency = frequency
        self.amplitude = amplitude
        self.samplerate = samplerate
        self.period = 1/frequency
        self.B=2*math.pi*frequency
        self.interval = 1/samplerate
        self.wrap = wrap+1
    def make_sin_wav(self):
        x=0
        v=self.amplitude*(math.sin(self.B*x))
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
        x+=self.interval
        while x<(self.period+(self.interval)):
            vals+=', '
            v=self.amplitude*(math.sin(self.B*x))
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
            x+=self.interval
            
        return vals
    def make_square_wav(self):
        x=0
        vals=str(self.wrap)
        x+=self.interval
        while(x<(self.period+self.interval)/2):
            vals+=', '
            vals+=str(self.wrap)
            x+=self.interval
            
        while(x<(self.period+self.interval)):
            vals+=', '
            vals+="0"
            x+=self.interval
            
        return vals
    def make_triangle_wav(self):
        
        tan_theta=4*(self.amplitude/self.period)
        x=0
        v=x*(tan_theta)
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
        x+=self.interval
        while(x<(self.period+self.interval)/4):
            vals+=', '
            v=x*(tan_theta)
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
            x+=self.interval
            
        tan_theta_down=self.period/(4*self.amplitude)
        while(x<(3*(self.period+self.interval)/4)):
            vals+=', '
            v=2*(self.amplitude)-(x/tan_theta_down)
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
            x+=self.interval
            
            
        while x<(self.period+(self.interval)):
            vals+=', '
            v=x*(tan_theta)
            vals+=str(int((v-3*self.amplitude)/(self.amplitude*2)*self.wrap))
            x+=self.interval
            
        return vals
    def make_sawtooth_wav(self):
        tan_theta=2*(self.amplitude/self.period)
        x=0
        v=x*(tan_theta)
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
        x+=self.interval
        while(x<(self.period+(self.interval))/2):
            vals+=', '
            v=x*(tan_theta)
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
            x+=self.interval
            
        while x<(self.period+(self.interval)):
            vals+=', '
            v=x*(tan_theta)
            vals+=str(int((v-self.amplitude)/(self.amplitude*2)*self.wrap))
            x+=self.interval
        return vals
    def make_reversed_sawtooth_wav(self):
        tan_theta_down=self.period/(2*self.amplitude)
        x=0
        v=(self.amplitude)-(x/tan_theta_down)
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
        x+=self.interval
        # while(x<(self.period+self.interval)/2):
        #     vals+=', '
        #     v=(self.amplitude)-(x/tan_theta_down)
        #     vals+=str(int((v+self.amplitude)/(self.amplitude*2)*255))
        #     x+=self.interval
        while x<(self.period+(self.interval)):
            vals+=', '
            v=(self.amplitude)-(x/tan_theta_down)
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
            x+=self.interval
            
        return vals
    def make_porabola_wav(self):
        x=0
        d=0
        v=self.amplitude*(2*self.frequency*(x-(self.period/2)))*(2*self.frequency*(x-(self.period/2)))
        vals=str(int(v*self.wrap))
        x+=self.interval
        while x<(self.period+(self.interval)):
            d+=1
            #print(d)
            vals+=', '
            v=self.amplitude*(2*self.frequency*(x-(self.period/2)))*(2*self.frequency*(x-(self.period/2)))
            vals+=str(int((v*self.wrap)))
            x+=self.interval
        return vals
    def make_harmonics(self):
        h=1
        harmonics=[]
        while h<=12:
            x=0
            B=(2*math.pi*(self.frequency)*(h+1))
            vals=str(int(self.wrap*(self.amplitude*(math.sin(B*x))+1)/2))
            x+=self.interval
            while x<((self.period)+(self.interval)):
                vals+=', '
                vals+=str(int(self.wrap*(self.amplitude*(math.sin(B*x))+1)/2))
                x+=self.interval
            harmonics.append(vals)
            h+=1
        return harmonics
f=open("waves.h","w")
Frequency=float(input("FREQUENCY: "))
samplerate=float(input("SAMPLERATE: "))
wrap=int(input("WRAP(default 99): "))
clockfreq=int(input("CLOCKSPEED(default 172800)kHz: "))
amplitude=1
wav = wave(Frequency,amplitude,samplerate,wrap)
sin=wav.make_sin_wav()
sqr=wav.make_square_wav()
tri=wav.make_triangle_wav()
saw=wav.make_sawtooth_wav()
rsaw=wav.make_reversed_sawtooth_wav()
pbla=wav.make_porabola_wav()
harmonics=wav.make_harmonics()
f.write("/* wave tables for a frequency of "+str(Frequency)+'\n')
f.write(" * with sampling rate of " + str(samplerate)+'\n')
f.write(" */\n")

f.write("#define WAV_DATA_LENGTH "+str(math.ceil(samplerate/Frequency))+'\n')
f.write("#define WAV_FREQUENCY "+str(Frequency)+'\n')
f.write('\n')
f.write('const float clkDiv='+str(round((clockfreq*100/(samplerate*(wrap+1))),3))+';\n')
f.write('uint16_t wrap='+str(wrap)+';\n')
f.write('int clockFreq='+str(clockfreq)+';\n\n')


f.write("uint8_t SIN_WAV_DATA[] = {\n")
f.write('    '+sin+'\n')
f.write('};\n')

f.write("uint8_t SQR_WAV_DATA[] = {\n")
f.write('    '+sqr+'\n')
f.write('};\n')

f.write("uint8_t TRI_WAV_DATA[] = {\n")
f.write('    '+tri+'\n')
f.write('};\n')

f.write("uint8_t SAW_WAV_DATA[] = {\n")
f.write('    '+saw+'\n')
f.write('};\n')

f.write("uint8_t R_SAW_WAV_DATA[] = {\n")
f.write('    '+rsaw+'\n')
f.write('};\n')

f.write("uint8_t PRBA_WAV_DATA[] = {\n")
f.write('    '+pbla+'\n')
f.write('};\n')

d=2
for harmonic in harmonics:
    f.write("uint8_t HARMONIC"+str(d)+"_WAV_DATA[] = {\n")
    f.write('    '+harmonic+'\n')
    f.write('};\n')
    d+=1
Frequency=Frequency/32
f.write("const float lowestFrequency="+str(round(((Frequency/2)*15/8),3))+";\n")
f.write("const float highestFrequency="+str(round(((Frequency*32)*16/15),3))+";\n")
f.write("const float freqList[]={\n")
for i in range(0,12):
    if(i==0):
        f.write("   "+str(1*Frequency)+", ")
    elif(i==1):
        f.write(str(round((Frequency*(16/15)),3))+", ")
    elif(i==2):
        f.write(str(round((Frequency*(9/8)),3))+", ")
    elif(i==3):
        f.write(str(round((Frequency*(6/5)),3))+", ")
    elif(i==4):
        f.write(str(round((Frequency*(5/4)),3))+", ")
    elif(i==5):
        f.write(str(round((Frequency*(4/3)),3))+", ")
    elif(i==6):
        f.write(str(round((Frequency*(45/32)),3))+", ")
    elif(i==7):
        f.write(str(round((Frequency*(3/2)),3))+", ")
    elif(i==8):
        f.write(str(round((Frequency*(8/5)),3))+", ")
    elif(i==9):
        f.write(str(round((Frequency*(5/3)),3))+", ")
    elif(i==10):
        f.write(str(round((Frequency*(9/5)),3))+", ")
    elif(i==11):
        f.write(str(round((Frequency*(15/8)),3))+", ")
Frequency=Frequency*2
for i in range(0,12):
    if(i==0):
        f.write(str(1*Frequency)+", ")
    elif(i==1):
        f.write(str(round((Frequency*(16/15)),3))+", ")
    elif(i==2):
        f.write(str(round((Frequency*(9/8)),3))+", ")
    elif(i==3):
        f.write(str(round((Frequency*(6/5)),3))+", ")
    elif(i==4):
        f.write(str(round((Frequency*(5/4)),3))+", ")
    elif(i==5):
        f.write(str(round((Frequency*(4/3)),3))+", ")
    elif(i==6):
        f.write(str(round((Frequency*(45/32)),3))+", ")
    elif(i==7):
        f.write(str(round((Frequency*(3/2)),3))+", ")
    elif(i==8):
        f.write(str(round((Frequency*(8/5)),3))+", ")
    elif(i==9):
        f.write(str(round((Frequency*(5/3)),3))+", ")
    elif(i==10):
        f.write(str(round((Frequency*(9/5)),3))+", ")
    elif(i==11):
        f.write(str(round((Frequency*(15/8)),3))+", ")

Frequency=Frequency*2
for i in range(0,12):
    if(i==0):
        f.write(str(1*Frequency)+", ")
    elif(i==1):
        f.write(str(round((Frequency*(16/15)),3))+", ")
    elif(i==2):
        f.write(str(round((Frequency*(9/8)),3))+", ")
    elif(i==3):
        f.write(str(round((Frequency*(6/5)),3))+", ")
    elif(i==4):
        f.write(str(round((Frequency*(5/4)),3))+", ")
    elif(i==5):
        f.write(str(round((Frequency*(4/3)),3))+", ")
    elif(i==6):
        f.write(str(round((Frequency*(45/32)),3))+", ")
    elif(i==7):
        f.write(str(round((Frequency*(3/2)),3))+", ")
    elif(i==8):
        f.write(str(round((Frequency*(8/5)),3))+", ")
    elif(i==9):
        f.write(str(round((Frequency*(5/3)),3))+", ")
    elif(i==10):
        f.write(str(round((Frequency*(9/5)),3))+", ")
    elif(i==11):
        f.write(str(round((Frequency*(15/8)),3))+", ")
Frequency=Frequency*2
for i in range(0,12):
    if(i==0):
        f.write(str(1*Frequency)+", ")
    elif(i==1):
        f.write(str(round((Frequency*(16/15)),3))+", ")
    elif(i==2):
        f.write(str(round((Frequency*(9/8)),3))+", ")
    elif(i==3):
        f.write(str(round((Frequency*(6/5)),3))+", ")
    elif(i==4):
        f.write(str(round((Frequency*(5/4)),3))+", ")
    elif(i==5):
        f.write(str(round((Frequency*(4/3)),3))+", ")
    elif(i==6):
        f.write(str(round((Frequency*(45/32)),3))+", ")
    elif(i==7):
        f.write(str(round((Frequency*(3/2)),3))+", ")
    elif(i==8):
        f.write(str(round((Frequency*(8/5)),3))+", ")
    elif(i==9):
        f.write(str(round((Frequency*(5/3)),3))+", ")
    elif(i==10):
        f.write(str(round((Frequency*(9/5)),3))+", ")
    elif(i==11):
        f.write(str(round((Frequency*(15/8)),3))+", ")

Frequency=Frequency*2
for i in range(0,13):
    if(i==0):
        f.write(str(1*Frequency)+", ")
    elif(i==1):
        f.write(str(round((Frequency*(16/15)),3))+", ")
    elif(i==2):
        f.write(str(round((Frequency*(9/8)),3))+", ")
    elif(i==3):
        f.write(str(round((Frequency*(6/5)),3))+", ")
    elif(i==4):
        f.write(str(round((Frequency*(5/4)),3))+", ")
    elif(i==5):
        f.write(str(round((Frequency*(4/3)),3))+", ")
    elif(i==6):
        f.write(str(round((Frequency*(45/32)),3))+", ")
    elif(i==7):
        f.write(str(round((Frequency*(3/2)),3))+", ")
    elif(i==8):
        f.write(str(round((Frequency*(8/5)),3))+", ")
    elif(i==9):
        f.write(str(round((Frequency*(5/3)),3))+", ")
    elif(i==10):
        f.write(str(round((Frequency*(9/5)),3))+", ")
    elif(i==11):
        f.write(str(round((Frequency*(15/8)),3))+", ")
    elif(i==12):
        f.write(str(round((Frequency*(2)),3))+"\n")
        f.write("};\n")
# f.write("uint8_t EVEN_HARMONICS_WAV_DATA[] = {\n")
# f.write('    '+even_harmonics+'\n')
# f.write('};\n')

# f.write("uint8_t ODD_HARMONICS_WAV_DATA[] = {\n")
# f.write('    '+odd_harmonics+'\n')
# f.write('};\n')
f.close()

