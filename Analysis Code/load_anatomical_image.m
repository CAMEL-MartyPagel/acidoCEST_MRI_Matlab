function [select_anatomical_image,anatomical_offsets]=load_anatomical_image(DIR_anatomical,select_anatomical_slice)

    % Obtain the number of echos and number of slices.
    cd(DIR_anatomical);
    method_file =fopen('method');
    line_number=0;
    while 1
        line_number=line_number+1;
        current_line = fgetl(method_file);
        if ~ischar(current_line),  break,   end
        if strncmp(current_line, '##$PVM_NEchoImages',18); 
            anatomical_number_of_echos = strread(current_line,'##$PVM_NEchoImages=%f');
        end
        if strncmp(current_line, '##$PVM_SPackArrNSlices',22); 
            current_line=fgetl(method_file);
            anatomical_number_of_slices=strread(current_line);
        end
    end
    fclose(method_file);
    
   % Get the image matrix dimensions, MHz value, and offsets
    DIR_pdata=strcat(DIR_anatomical,'\pdata\1\');
    cd(DIR_pdata)
    visu_pars_file=fopen('visu_pars');

    line_number=0;
    while 1
        line_number=line_number+1;
        current_line = fgetl(visu_pars_file);
        if ~ischar(current_line),   break,   end
        if strncmp(current_line, '##$VisuCoreSize',15); 
            current_line=fgetl(visu_pars_file);
            anatomical_image_size=strread(current_line);
            anatomical_rows=anatomical_image_size(2);
            anatomical_columns=anatomical_image_size(1);
        end
        if strncmp(current_line, '##$VisuCorePosition',19); 
            anatomical_position_linestart = line_number+1;
            current_line=fgetl(visu_pars_file);
            anatomical_offsets=strread(current_line);
        end
    end
    fclose(visu_pars_file);
    
    % Set up a 3D matrix for the images
    anatomical_images=zeros(anatomical_rows,anatomical_columns,anatomical_number_of_echos,anatomical_number_of_slices);
    
    % Get the image data from the 2dseq file.
    cd(DIR_pdata);
    FileID = fopen('2dseq','r','l');
    anatomical_images_from_2dseq = fread(FileID,'int16');
    fclose(FileID);
    
    anatomical_images=reshape(anatomical_images_from_2dseq,anatomical_columns,anatomical_rows,anatomical_number_of_echos,anatomical_number_of_slices);
    anatomical_images=permute(anatomical_images,[2 1 3 4]);
    select_anatomical_image=anatomical_images(:,:,1,select_anatomical_slice);
end
