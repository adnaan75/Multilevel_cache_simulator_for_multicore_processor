clc; clear all; close all;

y = [71.641	70.846	69.972; 55.476	53.434	52.986; 70.265	70.265	70.265];
names = {'D','E','F'};
X=[2 6 10; 22 26 30; 40 44 48];
figure(1)
b=bar(X,y);
b(1).BarWidth=2.8;
h=text(X(:,1),y(:,1),num2str(y(:,1)),'vert','bottom','horiz','right');

h=text(X(:,2),y(:,2),num2str(y(:,2)),'vert','bottom','horiz','center');

h=text(X(:,3),y(:,3),num2str(y(:,3)),'vert','bottom','horiz','left');


title('L2 read miss percentage');
set(gcf,'color','white');
set(gca,'fontsize',12);
names = {'mix1','mix2','mix3'};
set(gca,'xtick',[X(1,2) X(2,2) X(3,2)],'xticklabel',names)
ylabel('Percentage');
xlabel('Benchmarks');
legend('D','E','F','Location','north');
axis([X(1,1)-8 X(3,3)+8 0 80 ]);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
y = [0.046	0.034	0.030; 0.019	0.008	.008; 0.002	0.002	0.002];
names = {'D','E','F'};
X=[2 6 10; 22 26 30; 40 44 48];
figure(2)
b=bar(X,y);
b(1).BarWidth=2.8;
h=text(X(:,1),y(:,1),num2str(y(:,1)),'vert','bottom','horiz','right');

h=text(X(:,2),y(:,2),num2str(y(:,2)),'vert','bottom','horiz','center');

h=text(X(:,3),y(:,3),num2str(y(:,3)),'vert','bottom','horiz','left');


title('L2 Write miss percentage');
set(gcf,'color','white');
set(gca,'fontsize',12);
names = {'mix1','mix2','mix3'};
set(gca,'xtick',[X(1,2) X(2,2) X(3,2)],'xticklabel',names)
ylabel('Percentage');
xlabel('Benchmarks');
legend('D','E','F','Location','north');
axis([X(1,1)-8 X(3,3)+8 0 .05 ]);