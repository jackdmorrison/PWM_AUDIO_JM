import math

class wave:
    def __init__(self,frequency,amplitude,repeatRate,wrap):
        self.frequency = frequency
        self.amplitude = amplitude
        self.repeatRate = repeatRate
        self.period = 1/frequency
        self.B=2*math.pi*frequency
        self.interval = 1/repeatRate
        self.wrap = wrap
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
        vals=str(self.wrap*4/5)
        x+=self.interval
        #set array value to wrap size
        while(x<(self.period+self.interval)/2):
            vals+=', '
            vals+=str(self.wrap*4/5)
            x+=self.interval
        #set array value to 0
        while(x<(self.period+self.interval)):
            vals+=', '
            vals+="0"
            x+=self.interval
        #return array values as string
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
        
        while(x<(3*(self.period+self.interval)/4)):
            vals+=', '
            v=2*(self.amplitude)-(x*tan_theta)
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
        tan_theta=2*(self.amplitude/self.period)
        x=0
        v=(x*tan_theta)
        vals=str(int((self.amplitude-v)/(self.amplitude*2)*self.wrap))
        x+=self.interval
        while(x<(self.period+self.interval)/2):
            vals+=', '
            v=(x*tan_theta)
            vals+=str(int((self.amplitude-v)/(self.amplitude*2)*self.wrap))
            x+=self.interval
        while x<(self.period+(self.interval)):
            vals+=', '
            v=(x*tan_theta)
            vals+=str(int((3*self.amplitude-v)/(self.amplitude*2)*self.wrap))
            x+=self.interval
            
        return vals
    def make_reversed_sawtooth_wav(self):
        tan_theta=2*(self.amplitude/self.period)
        x=0
        v=(x*tan_theta)
        vals=str(int((self.amplitude-v)/(self.amplitude*2)*self.wrap))
        x+=self.interval
        while(x<(self.period+self.interval)/2):
            vals+=', '
            v=(x*tan_theta)
            vals+=str(int((self.amplitude-v)/(self.amplitude*2)*self.wrap))
            x+=self.interval
        while x<(self.period+(self.interval)):
            vals+=', '
            v=(x*tan_theta)
            vals+=str(int((3*self.amplitude-v)/(self.amplitude*2)*self.wrap))
            x+=self.interval
            
        return vals
    def make_porabola_wav(self):
        x=0
        #find duty cycle at x interval
        v=(2*self.frequency*(x-(self.period/2)))*(2*self.frequency*(x-(self.period/2)))
        #find level by multiplying by the wrap
        vals=str(int(v*self.wrap))
        x+=self.interval
        while x<(self.period+(self.interval)):
            vals+=', '
            #find duty cycle at x interval
            v=(2*self.frequency*(x-(self.period/2)))*(2*self.frequency*(x-(self.period/2)))
            #find level by multiplying by the wrap
            vals+=str(int((v*self.wrap)))
            x+=self.interval
        #return array values as string
        return vals
    #produce sine wave harmonics
    def make_harmonics(self):
        h=1
        harmonics=[]
        #loop through 12 times
        while h<=12:
            x=0
            #find B variable for harmonic h
            B=(2*math.pi*(self.frequency)*(h+1))
            #find level at x 
            vals=str(int(self.wrap*(self.amplitude*(math.sin(B*x))+1)/2))
            x+=self.interval
            #loop through until period of base frequency reached
            while x<((self.period)+(self.interval)):
                vals+=', '
                #find level at x 
                vals+=str(int(self.wrap*(self.amplitude*(math.sin(B*x))+1)/2))
                x+=self.interval
            #add string of array values to array
            harmonics.append(vals)
            h+=1
        #return array of arrays(as strings)
        return harmonics
valid=False
bit=8
def checkWrapAndRR(wrap,repeatRate):
    if(wrap>256):
        print("try reducing the wrap to less than 256\n")
        if(repeatRate>86400 or repeatRate<10800):
            print("and/or try changing the repeat rate to 10800-86400\n")
    elif(wrap<50):
        print("try increase the wrap to above 50\n")
        if(repeatRate>86400 or repeatRate<10800):
            print("and/or try changing the repeat rate to 10800-86400\n")
    else:
        if(repeatRate>86400 or repeatRate<10800):
            print("try changing the repeat rate to 10800-86400\n")
def checkClockDiv(clockfreq, repeatRate,wrap,Frequency):
    clkdiv=round((clockfreq*1000/(repeatRate*4*(wrap+1))))
    print("clock Divider:"+str(clkdiv))
    if(clkdiv<2.5):
        checkWrapAndRR(wrap,repeatRate)
    elif(clkdiv>256):
        checkWrapAndRR(wrap,repeatRate)
    highestFreq=(Frequency*8)*16/15
    lowestFreq=(Frequency/8)*15/8
    if(round(Frequency/highestFreq*clkdiv)<1.5):
        print("highest Frequency cant be met\n")
    if(256<round(Frequency/lowestFreq*clkdiv)):
        print("lowest Frequency cant be met\n")
while (valid==False):
    #take input
    Frequency=float(input("FREQUENCY: "))
    repeatRate=float(input("Repeated Rate: "))
    wrap=int(input("WRAP(MAX 65536): "))
    clockfreq=int(input("CLOCKSPEED(125000-220000)kHz: "))
    #valid initially true
    valid=True
    if(clockfreq>220000):
        print("Clock frequency too large\n")
        checkClockDiv(clockfreq, repeatRate,wrap,Frequency)
        valid=False
    elif(clockfreq<125000):
        print("Clock frequency too small\n")
        checkClockDiv(clockfreq, repeatRate,wrap,Frequency)
        valid=False
    else:
        clkdiv=(clockfreq*1000/(repeatRate*4*(wrap+1)))
        print("clock Divider:"+str(clkdiv)+"\n")
        if(clkdiv<2.5):
            valid=False
            checkWrapAndRR(wrap,repeatRate)
        elif(clkdiv>256):
            valid=False
            checkWrapAndRR(wrap,repeatRate)
        
        highestFreq=(Frequency*8)*16/15
        lowestFreq=(Frequency/8)*15/8
        if(Frequency/highestFreq*clkdiv*4<1.875):
            valid=False
            print("highestFrequency cant be met\n")
        if(256<round(Frequency/lowestFreq*clkdiv/4)):
            valid=False
            print("lowest Frequency cant be met\n")
        if(wrap<256):
            bit=8
        elif(wrap<65536):
            bit=16
        else:
            print("wrap too large\n")
            valid=False
        if(repeatRate>86400 or repeatRate<10800):
            valid=False
            print("and/or try changing the repeat rate to 10800-86400\n")
    
amplitude=1
#define wave object
wav = wave(Frequency,amplitude,repeatRate,wrap)
#get string arrays using object
sin=wav.make_sin_wav()#sine wave
sqr=wav.make_square_wav()#square wave
tri=wav.make_triangle_wav()#triangle wave
saw=wav.make_sawtooth_wav()#sawtooth wave
rsaw=wav.make_reversed_sawtooth_wav()#reversed sawtooth wave
pbla=wav.make_porabola_wav()#porabola wave
harmonics=wav.make_harmonics()#array of harmonic waves

#open header file
f=open("configuration.h","w")
#write top comment
f.write("/* wave tables for a frequency of "+str(Frequency)+'\n')
f.write(" * with repeating rate of " + str(repeatRate)+'\n')
f.write(" */\n")

#write the array length and wave frequency as macros
f.write("#define WAV_DATA_LENGTH "+str(math.ceil(repeatRate/Frequency))+'\n')
f.write("#define WAV_FREQUENCY "+str(Frequency)+'\n')
f.write('\n')
#define constants for clock divider, wrap and clock frequency
f.write('const float clkDiv='+str(round((clockfreq*1000/(repeatRate*4*(wrap+1))),3))+';\n')
f.write('const uint16_t wrap='+str(wrap)+';\n')
f.write('const int clockFreq='+str(clockfreq)+';\n\n')

#write sine wave array
f.write("uint"+str(bit)+"_t SIN_WAV_DATA[] = {\n")
f.write('    '+sin+'\n')
f.write('};\n')

#write square wave array 
f.write("uint"+str(bit)+"_t SQR_WAV_DATA[] = {\n")
f.write('    '+sqr+'\n')
f.write('};\n')
#write triangle wave array
f.write("uint"+str(bit)+"_t TRI_WAV_DATA[] = {\n")
f.write('    '+tri+'\n')
f.write('};\n')

#write sawtooth and reverse sawtooth array
f.write("uint"+str(bit)+"_t SAW_WAV_DATA[] = {\n")
f.write('    '+saw+'\n')
f.write('};\n')
f.write("uint"+str(bit)+"_t R_SAW_WAV_DATA[] = {\n")
f.write('    '+rsaw+'\n')
f.write('};\n')
#write porabola array
f.write("uint"+str(bit)+"_t PRBA_WAV_DATA[] = {\n")
f.write('    '+pbla+'\n')
f.write('};\n')
d=1
#write each harmonic array in harmonics array array
for harmonic in harmonics:
    d+=1
    f.write("uint"+str(bit)+"_t HARMONIC"+str(d)+"_WAV_DATA[] = {\n")
    f.write('    '+harmonic+'\n')
    f.write('};\n')

#write lowest and highest frequency
f.write("const float lowestFrequency="+str(round(((Frequency/8)*15/8),3))+";\n")
f.write("const float highestFrequency="+str(round(((Frequency*8)*16/15),3))+";\n")
#reduce frequency down 2 Octaves
Frequency=Frequency/4
f.write("const float freqListJust[]={\n")

#function takes in perfect first and f(header file) as parameters 
def justTuning(Frequency,f,last):
    #iterate to 12 for each semitone
    for i in range(0,12):
        #perfect first
        if(i==0):
            f.write("   "+str(round((Frequency),3))+", ")
        #minor second
        elif(i==1):
            f.write(str(round((Frequency*(16/15)),3))+", ")
        #major second
        elif(i==2):
            f.write(str(round((Frequency*(9/8)),3))+", ")
        #minor third
        elif(i==3):
            f.write(str(round((Frequency*(6/5)),3))+", ")
        #perfect fourth
        elif(i==4):
            f.write(str(round((Frequency*(5/4)),3))+", ")
        #Tri Tone
        elif(i==5):
            f.write(str(round((Frequency*(4/3)),3))+", ")
        #perfect fifth 
        elif(i==6):
            f.write(str(round((Frequency*(45/32)),3))+", ")
        #minor sixth
        elif(i==7):
            f.write(str(round((Frequency*(3/2)),3))+", ")
        #major sixth
        elif(i==8):
            f.write(str(round((Frequency*(8/5)),3))+", ")
        #minor seventh
        elif(i==9):
            f.write(str(round((Frequency*(5/3)),3))+", ")
        #major seventh
        elif(i==10):
            f.write(str(round((Frequency*(7/4)),3))+", ")
        #Perfect eighth
        elif(i==11):
            f.write(str(round((Frequency*(15/8)),3))+", ")
    #1 Ocatave up if last octave
    if(last):
        f.write(str(round((Frequency*(2)),3))+"\n")
        f.write("};\n")
justTuning(Frequency,f,False)
justTuning(Frequency*2,f,False)
justTuning(Frequency*4,f,False)
justTuning(Frequency*8,f,False)
justTuning(Frequency*16,f,True)
f.write("const float freqListEqualT[]={\n")
f.write("   ")
def equal_temperament(Frequency,f,last):
    for i in range(0,12):
        f.write(str(round((Frequency*(2**(i/12))),3))+", ")
    #1 Ocatave up if last 
    if(last):
        f.write(str(round((Frequency*(2)),3))+"\n")
        f.write("};\n")
equal_temperament(Frequency,f,False)
equal_temperament(Frequency*2,f,False)
equal_temperament(Frequency*4,f,False)
equal_temperament(Frequency*8,f,False)
equal_temperament(Frequency*16,f,True)

f.close()

