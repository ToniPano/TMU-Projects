s = tf('s');
G_c = 2/(s^2+s+2); %1/(s^2+s+1);
%K = 10;
%G_c = K*G_c/(1+K*G_c);
Ts = 1;
G_ZOH = c2d(G_c, Ts, 'zoh');
G_Bil = c2d(G_c, Ts, 'tustin');
G_imp = c2d(G_c, Ts, 'impulse');
G_FOH = c2d(G_c, Ts, 'foh');
G_FOH_d = tf(G_FOH, 'InputDelay', Ts);

Tf = 20;

subplot(2,3,1);
hold on
[y_c, t_c1] = step(G_c, Tf);
[y_ZOH, t_d] = step(G_ZOH, Tf);
[y_Bil, t_d] = step(G_Bil, Tf);
[y_imp, t_c2] = step(G_imp, Tf);
[y_FOH, t_d] = step(G_FOH, Tf);
[y_FOH_d, t_d] = step(G_FOH_d, Tf);
stairs(t_c1,heaviside(t_c1),':k');
plot(t_c1, y_c,'--k');
%plot(t_c2, y_imp);
stairs(t_d, y_ZOH, '-b.');
plot(t_d, y_FOH, '-r.'); % FOH can't predict the future, delayed by 1 sample period
%plot(t_d, y_FOH_d, ':m');
legend({'step input','continuous','ZOH','FOH'},'Location','southeast');
title('ZOH matches continuous if -->');
hold off

subplot(2,3,4);
hold on
r = @(t) t.*(0<=t);
ramp_func = @(t) 3*r(t)
ramp = ramp_func(t_d);
ramp_c = ramp_func(t_c1);
%ramp_slope = 3;
%ramp = ramp_slope*[0:Ts:Tf];
%[y_c, t_c1] = lsim(G_c, ramp);
%y_c = lsim(G_c, ramp_slope*t_c1, t_c1);
y_c = lsim(G_c, ramp_c, t_c1);
y_ZOH = lsim(G_ZOH, ramp);
y_Bil = lsim(G_Bil, ramp);
%[y_imp, t_c2] = step(G_imp, ramp);
y_FOH = lsim(G_FOH, ramp);
%t = ramp/ramp_slope;
plot(t_d,ramp,':k');
plot(t_c1, y_c,'--k');
stairs(t_d, y_ZOH, '-b.');
plot(t_d, y_FOH, '-r.');
legend({'ramp input','continuous','ZOH','FOH'},'Location','southeast');
title('FOH matches continuous if -->');
hold off

subplot(2,3,2);
hold on
u = @(t) (0<=t);
func = @(t) 0.2*u(t-1)+0.2*u(t-2)+0.4*u(t-3)+3.2*u(t-4)+1.5*u(t-5)-0.5*u(t-6)-4*u(t-8)+3*u(t-10);
%custom = [0,0.2, 0.4, 0.8, 4, 5.5, 5, 5, 5, 5,5];
custom = func(t_d);
t_c1 = [0:0.001:Tf];
custom_c = func(t_c1);
y_c = lsim(G_c, custom_c, t_c1);
y_ZOH = lsim(G_ZOH, custom);
y_FOH = lsim(G_FOH, custom);
stairs(t_d,custom,':k');
plot(t_c1, y_c,'--k');
stairs(t_d, y_ZOH, '-b.');
plot(t_d, y_FOH, '-r.');
legend({'custom step input','continuous','ZOH','FOH'},'Location','southeast');
title('input is only step functions');
hold off

subplot(2,3,5);
hold on
%slopes = @(t) (0.2*t).*((0<=t)&(t<2)) + (0.4*t-0.4).*((2<=t)&(t<3)) + (3.2*t-8.8).*((3<=t)&(t<4)) + (1.5*t-2).*((4<=t)&(t<5)) + (-0.5*t+8).*((5<=t)&(t<6)) + 5.*(6<=t);
slopes = @(t) 0.2*r(t) + 0.2*r(t-2) + 2.8*r(t-3) - 1.7*r(t-4) - 2*r(t-5) + 0.5*r(t-6);
custom = slopes(t_d);
custom_c = slopes(t_c1);
%bend = Tf/3;
%custom = [0,0.2, 0.4, 0.8, 4, 5.5, 5, 5, 5, 5,5];
%custom = [[0:Ts:bend],[Tf/3*ones(1,length([bend:Ts:Tf]))]];
%dT = t_c1(2)-t_c1(1);
%Tf_c = t_c1(length(t_c1));
%bend_c = Tf_c/3;
%custom_c = [[0:dT:bend_c],[Tf_c/3*ones(1,length([bend_c:dT:Tf_c]))]];
%custom_c = interp1(t_d, custom, t_c1, 'linear');
%[y_c, t_c1] = lsim(G_c, ramp);
y_c = lsim(G_c, custom_c, t_c1);
y_ZOH = lsim(G_ZOH, custom);
y_Bil = lsim(G_Bil, custom);
%[y_imp, t_c2] = step(G_imp, ramp);
y_FOH = lsim(G_FOH, custom);
plot(t_d,custom,':k');
plot(t_c1, y_c,'--k');
stairs(t_d, y_ZOH, '-b.');
plot(t_d, y_FOH, '-r.');
legend({'custom ramp input','continuous','ZOH','FOH'},'Location','southeast');
title('input is only linear ramps');
hold off
%legend('continuous','ZOH','Bilinear Transform','impulse','FOH');

subplot(2,3,3); %subplot(2,3, [3,6]);
hold on
u = @(t) (0<=t);
func = @(t) 0.5*r(t) - r(t-2) + r(t-3) - u(t-6) - 2*u(t-7) - 0.5*r(t-9) - 2*u(t-10) + u(t-11) + u(t-12) + 0.5*u(t-13);
%custom = [0,0.2, 0.4, 0.8, 4, 5.5, 5, 5, 5, 5,5];
custom = func(t_d);
t_c1 = [0:0.001:Tf];
custom_c = func(t_c1);
y_c = lsim(G_c, custom_c, t_c1);
y_ZOH = lsim(G_ZOH, custom);
y_FOH = lsim(G_FOH, custom);
plot(t_c1, custom_c,':k');
plot(t_c1, y_c,'--k');
stairs(t_d, y_ZOH, '-b.');
plot(t_d, y_FOH, '-r.');
xline(5, ':m', 'FOH match')
xline(14, ':m', 'ZOH match')
legend({'custom step & ramp input','continuous','ZOH','FOH'},'Location','southeast');
title('but none match when input is both');
hold off