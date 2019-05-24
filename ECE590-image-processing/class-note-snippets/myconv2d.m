function [output,full_output] = myconv2d(signal, filter)
    f_shape = size(filter);
    s_shape = size(signal);
    pad_btm = floor(f_shape(1)/2);
    pad_top = floor((f_shape(1)-1)/2);
    pad_right = floor(f_shape(2)/2);
    pad_left = floor((f_shape(2)-1)/2);
    filter_lr = fliplr(filter');
    pad_signal = padarray(signal, [pad_top, pad_left], 'pre');
    pad_signal = padarray(pad_signal, [pad_btm, pad_right], 'post');
    output = zeros(s_shape);
    for i = 1:s_shape(1)
        for j = 1:s_shape(2)
            for m = 1:f_shape(1)
                for n = 1:f_shape(2)
                    output(i,j) = output(i,j) + filter_lr(m,n) * pad_signal(i+m-1, j+n-1);
                end
            end
        end
    end
    
    full_output = zeros(size(pad_signal));
    for i = 1:s_shape(1)
        for j = 1:s_shape(2)
            for m = 1:f_shape(1)
                for n = 1:f_shape(2)
                    full_output(i+m-1, j+n-1) = full_output(i+m-1, j+n-1) + filter_lr(m,n) * signal(i, j);
                end
            end
        end
    end
end
