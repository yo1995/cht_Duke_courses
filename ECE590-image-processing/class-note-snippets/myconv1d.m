function [output,full_output] = myconv1d(signal, filter)
    f_len = length(filter);
    s_len = length(signal);
    pad_left = floor((f_len-1)/2);
    pad_right = floor(f_len/2);
    pad_signal = [zeros(1,pad_left), signal, zeros(1, pad_right)];
    filter_lr = fliplr(filter);
    output = zeros(1, length(signal));
    for i=1:s_len
        for j=1:length(filter_lr)
            output(i) = output(i) + filter_lr(j) * pad_signal(i+j-1);
        end
    end
    full_output = zeros(1, length(pad_signal));
    for i=1:s_len
        for j=1:length(filter_lr)
            full_output(i+j-1) = full_output(i+j-1) + filter_lr(j) * signal(i);
        end
    end
end