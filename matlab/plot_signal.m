load data;

signal_length = sum(durations);
x = 1:signal_length;
y = zeros(1, signal_length);

index = 1;
for i = 1:length(durations)
    value = values(i);
    for u = 1:durations(i)
        y(index) = value;
        index = index + 1;
    end
end

plot(x, y);
ylim([0 2]);