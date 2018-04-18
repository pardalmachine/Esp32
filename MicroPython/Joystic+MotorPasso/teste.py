import machine
import time
class ULN2003:
    Seqs = []
    pinos=[]
    def __init__(self,pino1,pino2,pino3,pino4):
        self.pinos.append(machine.Pin(pino1,machine.Pin.OUT))
        self.pinos.append(machine.Pin(pino2,machine.Pin.OUT))
        self.pinos.append(machine.Pin(pino3,machine.Pin.OUT))
        self.pinos.append(machine.Pin(pino4,machine.Pin.OUT))
        self.Seqs.append([1,0,0,0])
        self.Seqs.append([1,1,0,0])
        self.Seqs.append([0,1,0,0])
        self.Seqs.append([0,1,1,0])
        self.Seqs.append([0,0,1,0])
        self.Seqs.append([0,0,1,1])
        self.Seqs.append([0,0,0,1])
        self.Seqs.append([1,0,0,1])


    @classmethod
    def Gira(self, quantidade, direcao, intervalo):
        sequencias=[]
        if direcao:
            for sq1 in self.Seqs[::-1]:
                sequencias.append(sq1)
        else:
            for sq2 in self.Seqs:
                sequencias.append(sq2)
        for x in range(quantidade):
            for sequencia in sequencias:
                for i in range(4):
                    self.pinos[i].value(sequencia[i])
                time.sleep(intervalo)

driver = ULN2003(15,2,4,5)

#print('Horario')
#driver.Gira(200,True,0.0011)

#time.sleep(0.5)

#print('anti-horario')
#driver.Gira(200,False,0.0011)
adc = machine.ADC(machine.Pin(36))
adc.atten(adc.ATTN_11DB)
adc.width(adc.WIDTH_12BIT)

while True:
    time.sleep(0.02)
    valor=adc.read()
    direcaohoraria=True
    intervalo=0.0011
    if valor<1600:
        direcaohoraria=False
        intervalo=0.0030
        if valor<10:
            intervalo=0.0011
        driver.Gira(5,direcaohoraria,intervalo)
    if valor>2500:
        direcaohoraria=True
        intervalo=0.0030
        if valor>4000:
            intervalo=0.0011
        driver.Gira(5,direcaohoraria,intervalo)

