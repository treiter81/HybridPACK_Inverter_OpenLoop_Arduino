%clear 

doplot = 1;

steps = 1024;
angle = linspace(0,2*pi,steps+1);
angle = angle(1:end-1);

THISPWMDu = zeros(steps,1);
THISPWMDv = zeros(steps,1);
THISPWMDw = zeros(steps,1);
THISPWMTHI = zeros(steps,1);

for i=1:length(angle)
    M=1;
    
    THISPWMTHI(i) = 0.16714*sin(3*angle(i)+0.5*pi()) * 1/2;

    THISPWMDu(i) = sin(angle(i)+0.5*pi()) * M/2 - THISPWMTHI(i);
    THISPWMDv(i) = sin(angle(i)+0.5*pi()+(4*pi()/3)) * M/2 - THISPWMTHI(i);
    THISPWMDw(i) = sin(angle(i)+0.5*pi()+(2*pi()/3)) * M/2 - THISPWMTHI(i);
end

%Du = fixdt(1,16,2)
fixedPointValue = fi(THISPWMDu, 1, 16, 14);
THISPWMDufi = storedInteger(fixedPointValue);

fixedPointValue = fi(THISPWMDv, 1, 16, 14);
THISPWMDvfi = storedInteger(fixedPointValue);

fixedPointValue = fi(THISPWMDw, 1, 16, 14);
THISPWMDwfi = storedInteger(fixedPointValue);

fixedPointValue = fi(THISPWMTHI, 1, 16, 14);
THISPWMTHIfi = storedInteger(fixedPointValue);

%THISPWMsteps = steps;
%THISPWMangle = angle;

if doplot ==1

figure
hold on
title('ThirdHarmonic SPWM; double; real value')
plot(rad2deg(angle),THISPWMDu,'b')
plot(rad2deg(angle),THISPWMDv,'r')
plot(rad2deg(angle),THISPWMDw,'g')
plot(rad2deg(angle),THISPWMTHI,'k')
hl(1) = legend;
grid on

figure
hold on
title('ThirdHarmonic SPWM; fixed point; stored integer')
plot(1:length(THISPWMDufi),THISPWMDufi,'b')
plot(1:length(THISPWMDvfi),THISPWMDvfi,'r')
plot(1:length(THISPWMDwfi),THISPWMDwfi,'g')
hl(2) = legend;
grid on

hl(1).String = {'U','V','W'};
hl(2).String = {'U','V','W'};
end

clear fixedPointValue hl steps M i doplot