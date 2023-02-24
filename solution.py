import math
class wave:
    def __init__(self,frequency,amplitude,samplerate):
        self.frequency = frequency
        self.amplitude = amplitude
        self.samplerate = samplerate
        self.period = 1/frequency
        self.B=2*math.pi*frequency
        self.interval = 1/samplerate
    def make_sin_wav(self):
        x=0
        v=self.amplitude*(math.sin(self.B*x))
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*255))
        x+=self.interval
        while x<(self.period+(self.interval)):
            vals+=', '
            v=self.amplitude*(math.sin(self.B*x))
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*255))
            x+=self.interval
            
        return vals
    def make_square_wav(self):
        x=0
        vals="255"
        x+=self.interval
        while(x<(self.period+self.interval)/2):
            vals+=', '
            vals+="255"
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
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*255))
        x+=self.interval
        while(x<(self.period+self.interval)/4):
            vals+=', '
            v=x*(tan_theta)
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*255))
            x+=self.interval
            
        tan_theta_down=self.period/(4*self.amplitude)
        while(x<(3*(self.period+self.interval)/4)):
            vals+=', '
            v=2*(self.amplitude)-(x/tan_theta_down)
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*255))
            x+=self.interval
            
            
        while x<(self.period+(self.interval)):
            vals+=', '
            v=x*(tan_theta)
            vals+=str(int((v-3*self.amplitude)/(self.amplitude*2)*255))
            x+=self.interval
            
        return vals
    def make_sawtooth_wav(self):
        tan_theta=2*(self.amplitude/self.period)
        x=0
        v=x*(tan_theta)
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*255))
        x+=self.interval
        while(x<(self.period+(self.interval))/2):
            vals+=', '
            v=x*(tan_theta)
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*255))
            x+=self.interval
            
        while x<(self.period+(self.interval)):
            vals+=', '
            v=x*(tan_theta)
            vals+=str(int((v-self.amplitude)/(self.amplitude*2)*255))
            x+=self.interval
        return vals
    def make_reversed_sawtooth_wav(self):
        tan_theta_down=self.period/(2*self.amplitude)
        x=0
        v=(self.amplitude)-(x/tan_theta_down)
        vals=str(int((v+self.amplitude)/(self.amplitude*2)*255))
        x+=self.interval
        # while(x<(self.period+self.interval)/2):
        #     vals+=', '
        #     v=(self.amplitude)-(x/tan_theta_down)
        #     vals+=str(int((v+self.amplitude)/(self.amplitude*2)*255))
        #     x+=self.interval
        while x<(self.period+(self.interval)):
            vals+=', '
            v=(self.amplitude)-(x/tan_theta_down)
            vals+=str(int((v+self.amplitude)/(self.amplitude*2)*255))
            x+=self.interval
            
        return vals
    def make_porabola_wav(self):
        x=0
        d=0
        v=self.amplitude*(2*self.frequency*(x-(self.period/2)))*(2*self.frequency*(x-(self.period/2)))
        vals=str(int(v*255))
        x+=self.interval
        while x<(self.period+(self.interval)):
            d+=1
            print(d)
            vals+=', '
            v=self.amplitude*(2*self.frequency*(x-(self.period/2)))*(2*self.frequency*(x-(self.period/2)))
            vals+=str(int((v*255)))
            x+=self.interval
        return vals
f=open("waves.h","w")
Frequency=256
samplerate=44000
amplitude=1
wav = wave(Frequency,amplitude,samplerate)
sin=wav.make_sin_wav()
sqr=wav.make_square_wav()
tri=wav.make_triangle_wav()
saw=wav.make_sawtooth_wav()
rsaw=wav.make_reversed_sawtooth_wav()
pbla=wav.make_porabola_wav()
f.write("/* wave tables for a frequency of "+str(Frequency)+'\n')
f.write(" * with sampling rate of " + str(samplerate)+'\n')
f.write(" */\n")
f.write("#define WAV_DATA_LENGTH "+str(math.ceil(samplerate/Frequency))+'\n')
f.write("#define WAV_FREQUENCY "+str(Frequency)+'\n')
f.write('\n')
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
f.close()

