%clear 

doplot = 1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

SVPWMDu = zeros(steps,1);
SVPWMDv = zeros(steps,1);
SVPWMDw = zeros(steps,1);

%%%%%%%%
%SVPWM CALCULATION
%%%%%%%%
for i=1:length(angle)
    M=1;

    sector = floor(angle(i) / (pi()/3)) + 1;
    %   calculate the duty cycles
    T1 = (3^0.5) * sin(sector*(pi()/3) - angle(i)) * M/2;
    T2 = (3^0.5) * sin(angle(i) - (sector-1.0)*(pi()/3)) * M/2;
    T0 = 1 - T1 - T2;
    
    % calculate the duty cycles(times)
    switch(sector)
        case 1
            Du = T1 + T2 + T0/2;
            Dv = T2 + T0/2;
            Dw = T0/2;
            %break;
        case 2
            Du = T1 +  T0/2;
            Dv = T1 + T2 + T0/2;
            Dw = T0/2;
            %break;
        case 3
            Du = T0/2;
            Dv = T1 + T2 + T0/2;
            Dw = T2 + T0/2;
            %break;
        case 4
            Du = T0/2;
            Dv = T1+ T0/2;
            Dw = T1 + T2 + T0/2;
            %break;
        case 5
            Du = T2 + T0/2;
            Dv = T0/2;
            Dw = T1 + T2 + T0/2;
            %break;
        case 6
            Du = T1 + T2 + T0/2;
            Dv = T0/2;
            Dw = T1 + T0/2;
            %break;
        otherwise       % possible error state
            Du = 0;
            Dv = 0;
            Dw = 0;
    end

    SVPWMDu(i) = Du;
    SVPWMDv(i) = Dv;
    SVPWMDw(i) = Dw;

end

SVPWMDu=SVPWMDu-0.5;
SVPWMDv=SVPWMDv-0.5;
SVPWMDw=SVPWMDw-0.5;

clear sector Du Dv Dw T0 T1 T2
%%%%%%%%
%END SVPWM CALCULATION
%%%%%%%%


fixedPointValue = fi(SVPWMDu, 1, 16, 14);
SVPWMDufi = storedInteger(fixedPointValue);

fixedPointValue = fi(SVPWMDv, 1, 16, 14);
SVPWMDvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(SVPWMDw, 1, 16, 14);
SVPWMDwfi = storedInteger(fixedPointValue);

%SVPWMsteps = steps;
%SVPWMangle = angle;

if doplot ==1

figure
hold on
title('SVPWM; double; real value')
plot(rad2deg(angle),SVPWMDu,'b')
plot(rad2deg(angle),SVPWMDv,'r')
plot(rad2deg(angle),SVPWMDw,'g')
hl(1) = legend;
grid on



figure
hold on
title('SVPWM; fixed point; stored integer')
plot(1:length(SVPWMDufi),SVPWMDufi,'b')
plot(1:length(SVPWMDvfi),SVPWMDvfi,'r')
plot(1:length(SVPWMDwfi),SVPWMDwfi,'g')
hl(2) = legend;
grid on


plot(uint16(1),0,'b+')
plot(uint16(1 + (2*pi()/3)*1024/(2*pi())),0,'r+')
plot(uint16(1 + (4*pi()/3)*1024/(2*pi())),0,'g+')

plot(uint16(1 + 1*pi()*1024/(2*pi())),0,'b*')
plot(uint16(1 + (5*pi()/3)*1024/(2*pi())),0,'r*')
plot(uint16(1 + (1*pi()/3)*1024/(2*pi())),0,'g*')

hl(1).String = {'U','V','W'};
hl(2).String = {'U','V','W'};

end
clear fixedPointValue hl steps M i doplot