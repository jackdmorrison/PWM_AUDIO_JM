import math
class wave:
    def __init__(self,frequency,amplitude,repeatRate,wrap):
        self.frequency = frequency
        self.amplitude = amplitude
        self.repeatRate = repeatRate
        self.period = 1/frequency
        self.B=2*math.pi*frequency
        self.interval = 1/repeatRate
        self.wrap = wrap+5
    def make_sin_wav(self):
        x=0
        #v value is the duty cycle at given point x
        v=self.amplitude*(math.sin(self.B*x))
        #store value at string justifying it to the middle of the wrap size
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
        #adding 1 interval
        x+=self.interval
        #loops until the x point is above the period length
        while x<(self.period+(self.interval)):
            vals+=', '
            v=self.amplitude*(math.sin(self.B*x))
            #concatenate value
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*self.wrap))
            x+=self.interval
        #returns the string
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
valid=False
bit=8
def checkWrapAndRR(wrap,repeatRate):
    if(wrap>256):
        print("try reducing the wrap to less than 256\n")
        if(10800>repeatRate>86400):
            print("and/or try changing the repeat rate to 10800-86400\n")
    elif(wrap<50):
        print("try increase the wrap to above 50\n")
        if(10800>repeatRate>86400):
            print("and/or try changing the repeat rate to 10800-86400\n")
    else:
        if(10800>repeatRate>86400):
            print("try changing the repeat rate to 10800-86400\n")
def checkClockDiv(clockfreq, repeatRate,wrap):
    if(round((clockfreq*1000/(repeatRate*4*(wrap+1))))<2.5):
            checkWrapAndRR(wrap,repeatRate)
    elif(round((clockfreq*1000/(repeatRate*4*(wrap+1))))>256):
            checkWrapAndRR(wrap,repeatRate)
while (valid==False):
    Frequency=float(input("FREQUENCY: "))
    repeatRate=float(input("Repeated Rate: "))
    wrap=int(input("WRAP(MAX 65536): "))
    clockfreq=int(input("CLOCKSPEED(125000-220000)kHz: "))
    valid=True
    if(clockfreq>220000):
        print("Clock frequency too large\n")
        checkClockDiv(clockfreq, repeatRate,wrap)
        valid=False
    elif(clockfreq<125000):
        print("Clock frequency too small\n")
        checkClockDiv(clockfreq, repeatRate,wrap)
        valid=False
    elif(round((clockfreq*1000/(repeatRate*4*(wrap+1))))<2.5):
        valid=False
        checkWrapAndRR(wrap,repeatRate)
    elif(round((clockfreq*1000/(repeatRate*4*(wrap+1))))>256):
        valid=False
        checkWrapAndRR(wrap,repeatRate)
    elif(10800>repeatRate or repeatRate>86400):
        print("try changing the repeat rate to 10800-86400\n")
    elif(500<math.ceil(repeatRate/Frequency)):
        valid=False
        print("you should probably increase the base frequency\n")
    elif(math.ceil(repeatRate/Frequency)<20):
        valid=False
        print("you should probably decrease the base frequency\n")
    elif(wrap<256):
        bit=8
    elif(wrap<65536):
        bit=16
    else:
        print("wrap too large\n")
        valid=False

    
            
    if(10800>repeatRate or repeatRate>86400):
        print("try changing the repeat rate to 10800-86400\n")
    elif(500<math.ceil(repeatRate/Frequency)):
        valid=False
        print("you should probably increase the base frequency\n")
    elif(math.ceil(repeatRate/Frequency)<20):
        print("you should probably decrease the base frequency\n")
    
amplitude=1
wav = wave(Frequency,amplitude,repeatRate,wrap)
sin=wav.make_sin_wav()
sqr=wav.make_square_wav()
tri=wav.make_triangle_wav()
saw=wav.make_sawtooth_wav()
rsaw=wav.make_reversed_sawtooth_wav()
pbla=wav.make_porabola_wav()
harmonics=wav.make_harmonics()
f.write("/* wave tables for a frequency of "+str(Frequency)+'\n')
f.write(" * with repeating rate of " + str(repeatRate)+'\n')
f.write(" */\n")

f.write("#define WAV_DATA_LENGTH "+str(math.ceil(repeatRate/Frequency))+'\n')
f.write("#define WAV_FREQUENCY "+str(Frequency)+'\n')
f.write('\n')
f.write('const float clkDiv='+str(round((clockfreq*1000/(repeatRate*4*(wrap+1))),3))+';\n')
f.write('uint16_t wrap='+str(wrap)+';\n')
f.write('int clockFreq='+str(clockfreq)+';\n\n')


f.write("uint"+str(bit)+"_t SIN_WAV_DATA[] = {\n")
f.write('    '+sin+'\n')
f.write('};\n')

f.write("uint"+str(bit)+"_t SQR_WAV_DATA[] = {\n")
f.write('    '+sqr+'\n')
f.write('};\n')

f.write("uint"+str(bit)+"_t TRI_WAV_DATA[] = {\n")
f.write('    '+tri+'\n')
f.write('};\n')

f.write("uint"+str(bit)+"_t SAW_WAV_DATA[] = {\n")
f.write('    '+saw+'\n')
f.write('};\n')

f.write("uint"+str(bit)+"_t R_SAW_WAV_DATA[] = {\n")
f.write('    '+rsaw+'\n')
f.write('};\n')

f.write("uint"+str(bit)+"_t PRBA_WAV_DATA[] = {\n")
f.write('    '+pbla+'\n')
f.write('};\n')

d=2
for harmonic in harmonics:
    f.write("uint"+str(bit)+"_t HARMONIC"+str(d)+"_WAV_DATA[] = {\n")
    f.write('    '+harmonic+'\n')
    f.write('};\n')
    d+=1
Frequency=Frequency/4
f.write("const float lowestFrequency="+str(round(((Frequency/2)*15/8),3))+";\n")
f.write("const float highestFrequency="+str(round(((Frequency*32)*16/15),3))+";\n")
f.write("const float freqListJust[]={\n")
def justTuning(Frequency,f,last):
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
    #1 Ocatave up if last 
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

