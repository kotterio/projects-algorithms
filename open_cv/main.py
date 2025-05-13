import cv2

import numpy as np


def Theta(edge1, edge2):
    v1_u = edge1 / np.linalg.norm(edge1)
    v2_u = edge2 / np.linalg.norm(edge2)
    return np.arccos(np.clip(np.dot(v1_u, v2_u), -1.0, 1.0))
def Lendif(edge1, edge2):
    len1 = np.linalg.norm(edge1)
    len2 = np.linalg.norm(edge2)
    return abs(len2 - len1)

def Find(massiv, x, y):
    poradok = []
    for i in range(len(massiv)):
      r, l = massiv[i]
      next_x, next_y = massiv[(i+1)%6]
      next_x_1, next_y_1 = massiv[(i+2)%6]
      edge1 = [(x-r), (y-l)]
      edge2 = [(next_x - next_x_1), (next_y - next_y_1)]
      theta = Theta(edge1, edge2)
      lendif = Lendif(edge1, edge2)
      poradok.append([r, l, theta, lendif, i])
    min_theta_len = 1000000
    x_edge = 0
    y_edge = 0
    u = 0
    for j in range(len(poradok)):
      if((poradok[j][2] + poradok[j][3]) < min_theta_len):
          min_theta_len = poradok[j][2] + poradok[j][3]
          x_edge = poradok[j][0]
          y_edge = poradok[j][1]
          u = poradok[j][4]
    return [x_edge, y_edge, u]
def find_paralel(x1, y1, x2, y2, corne):
    edge = [(x2-x1), (y2 - y1)] 
    thetas = []
    for i in range(len(corne)):
        f_1, s_1 = corne[i]
        f_2, s_2 = corne[(i+1)%6]
        edge_1 = [(f_2 - f_1), (s_2 - s_1)]
        thetas.append([Theta(edge, edge_1), i])
    sorted_data = sorted(thetas, key=lambda x: x[0])   
    return sorted_data

def Size(edge):
    return(np.sqrt(edge[0]**2 + edge[1]**2))

def find_true_point1(x1, y1, x2, y2, corne, image):
    sort = find_paralel(x1, y1, x2, y2, corne)
    sort = find_paralel(x1, y1, x2, y2, corne)
    edge1 = [(x1-x2), (y1 - y2)]
    edge2 = [(x2-x1), (y2 - y1)]
    true_point_index = 0
    first_index = (sort[0][1] -1)%6
    second_index = (sort[0][1] + 2)%6
    edge_11 = [(corne[first_index][0]-x2), (corne[first_index][1]-y2)]
    edge_12 = [(corne[first_index][0]-x1), (corne[first_index][1]-y1)]
    edge_21 = [(corne[second_index][0] - x2), (corne[second_index][1] - y2)]
    edge_22 = [(corne[second_index][0] - x1), (corne[second_index][1] - y1)]
    first_angle1 = Theta(edge1, edge_11)
    first_angle2 = Theta(edge1, edge_12)
    first_angle = min(first_angle2, first_angle1)
    second_angle1 = Theta(edge2, edge_21)
    second_angle2 = Theta(edge2, edge_22)
    second_angle = min(second_angle2, second_angle1)
    if (first_angle < second_angle):
        true_point_index = (sort[0][1] -1)%6
    else:
        true_point_index = (sort[0][1] + 2)%6
    edge_3 = [(corne[true_point_index][0] - x1), (corne[true_point_index][1] - y1)]
    edge_4 = [(corne[true_point_index][0] - x2), (corne[true_point_index][1] - y2)]
    cv2.circle(image, (x1, y1), 5, (108, 108, 108), -1)
    cv2.circle(image, (x2, y2), 5, (108, 108, 108), -1)
    cv2.circle(image, (corne[first_index][0], corne[first_index][1]), 5, (255, 255, 108), -1) 
    cv2.circle(image, (corne[second_index][0], corne[second_index][1]), 5, (255, 255, 255), -1)
    cv2.imwrite('paralel.png', image)
    if (Size(edge_3) > Size(edge_4)):
        return[[x1, y1], [x2, y2]]
    else:
        return[[x2, y2], [x1, y1]]

def find_true_point(x1, y1, x2, y2, corne, image):
    sort = find_paralel(x1, y1, x2, y2, corne)
    edge = [(x1-x2), (y1 - y2)]
    true_point_index = 0
    first_index = (sort[0][1] -1)%6
    second_index = (sort[0][1] + 2)%6
    edge_1 = [(corne[first_index][0]-x2), (corne[first_index][1]-y2)]
    edge_2 = [(corne[second_index][0] - x2), (corne[second_index][1] - y2)]
    first_angle = Theta(edge, edge_1)
    second_angle = Theta(edge, edge_2)
    if (first_angle < second_angle):
        true_point_index = (sort[0][1] -1)%6
    else:
        true_point_index = (sort[0][1] + 2)%6
    edge_3 = [(corne[true_point_index][0] - x1), (corne[true_point_index][1] - y1)]
    edge_4 = [(corne[true_point_index][0] - x2), (corne[true_point_index][1] - y2)]
    if (Size(edge_3) > Size(edge_4)):
        return[[x1, y1], [x2, y2]]
    else:
        return[[x2, y2], [x1, y1]]

# Загружаем изображение
image = cv2.imread('куб.jpg')
clone3 = image.copy()
clone_image = image
clone_for_edges = image
gray = cv2.cvtColor(image, cv2.COLOR_RGB2GRAY)
cv2.imwrite('cube_corners1.png', gray)
kernel_size = 5
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (kernel_size, kernel_size)) 
opened_image = cv2.dilate(gray, kernel, iterations=2) 
cv2.imwrite('dilalite.png',opened_image)
eroded_image = cv2.erode(opened_image, kernel, iterations=3)
cv2.imwrite('erose.png', eroded_image)
blurred = cv2.GaussianBlur(eroded_image, (5, 5), 0)
cv2.imwrite('cube_corners3.png', blurred)
edges = cv2.Canny(blurred, 50, 150)
cv2.imwrite('edges.png', edges)
contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# 3. Создание копии изображения
image_copy = image.copy()

# 4. Заранее заданные цвета
colors = [(255, 255, 0), (0, 255, 0), (0, 0, 255), (255, 255, 0), (255, 0, 255)]  # BGR

# 5. Рисование контуров с использованием заранее заданных цветов
for i, contour in enumerate(contours):
    color = colors[i % len(colors)]  # Циклический выбор цвета
    cv2.drawContours(image_copy, [contour], -1, color, 2)
cv2.imwrite('Image with Contours.png', image_copy)


for contour in contours:
    epsilon = 0.02 * cv2.arcLength(contour, True)
    approx = cv2.approxPolyDP(contour, epsilon, True)
    image_copy1 = image.copy()
    cv2.drawContours(image_copy1, [approx], -1, (0,255,0), 2)
    cv2.imwrite('Image with approx.png', image_copy1)
    final = []
    if (len(approx)) == 4:
        x_im, y_im, w, h = cv2.boundingRect(approx)  
        x_im = x_im - 20
        y_im = y_im - 20
        w += 40
        h +=40
        cube1 = image[y_im:y_im+h, x_im:x_im+w]
        cube = cube1
        cv2.imwrite('Вырезанная область.png', cube)
        resized_cube = cv2.resize(cube, (600, 600), interpolation=cv2.INTER_LINEAR)
        resized_cube2 = resized_cube.copy()
        gray_cube = cv2.cvtColor(resized_cube, cv2.COLOR_BGR2GRAY)
        opened_image_cube = cv2.dilate(gray_cube, kernel, iterations=2)
        cv2.imwrite('dilalite_cube.png',opened_image_cube)
        eroded_image_cube = cv2.erode(opened_image_cube, kernel, iterations=3)
        cv2.imwrite('erose_cube.png', eroded_image_cube)
        blurred_cube = cv2.GaussianBlur(eroded_image_cube, (5, 5), 0)
        cv2.imwrite('cube_corners3_cube.png', blurred_cube)
        corners_original = approx.reshape(-1, 2)
        corners_cropped = corners_original - [x_im, y_im]
        scale_x = 600 / w
        scale_y = 600 / h
        corners_resized = corners_cropped * [scale_x, scale_y]
        corners_resized = np.round(corners_resized).astype(np.int32)
        exclusion_radius = 70
        mask1 = np.zeros((600, 600), dtype=np.uint8)
        cv2.drawContours(mask1, [corners_resized], -1, 255, cv2.FILLED)
        cv2.imwrite('mask.png', mask1)
        for x, y in corners_resized:
          cv2.circle(mask1, (int(x), int(y)), exclusion_radius, 0, -1)
        corners = cv2.goodFeaturesToTrack(blurred_cube, maxCorners=4, qualityLevel=0.1, minDistance=50, mask = mask1) 
        final_res = []
        if (len(corners) == 4):
            for i in corners:
                x, y = i.ravel()
                cv2.circle(resized_cube, (int(x), int(y)), 5, (0, 255, 0), -1)
                cv2.imwrite('inside_points.png', resized_cube)
                x_cube = (x / 600) * w
                y_cube = (y / 600) * h
                x_original = x_im + x_cube
                y_original = y_im + y_cube
                x_original = int(round(x_original))
                y_original = int(round(y_original))
                final_res.append([x_original, y_original])
            x_neigh = final_res[0][0]
            y_neihg = final_res[0][1]
            ro_min = 1000000
            ro_max = 0
            index_i_min = 0
            index_i_max = 0
            pairs = []
            for i in range(1, len(final_res)):
                a, b = final_res[i]
                if (np.sqrt((a-x_neigh)**2 + (b-y_neihg)**2) < ro_min):
                    ro = np.sqrt((a-x_neigh)**2 + (b-y_neihg)**2)
                    index_i_min = i
            for i in range(1, len(final_res)):
                a, b = final_res[i]
                if (np.sqrt((a-x_neigh)**2 + (b-y_neihg)**2) > ro_max):
                    ro_max = np.sqrt((a-x_neigh)**2 + (b-y_neihg)**2)
                    index_i_max = i          
            for i in range(1, len(final_res)):
                if (i!= index_i_min and i != index_i_max):
                   pairs.append([x_neigh, y_neihg, final_res[i][0], final_res[i][1]]) 
            for i in range(1, len(final_res)):
                if (i == index_i_min or i == index_i_max):
                    continue
            #pairs.append((final_res[index_i_min][0], final_res[index_i_min][1], final_res[index_i_max][0], final_res[index_i_max][1]))
            temprory =[]
            for i in range(len(final_res)):
                x_min, y_min =-1, -1
                ro_min = 1000000
                for j in range(len(corners_original)):
                    ro_candidate = np.sqrt((final_res[i][0] - corners_original[j][0])**2 + (final_res[i][1] - corners_original[j][1])**2)
                    if ro_candidate < ro_min:
                        ro_min = ro_candidate
                        x_min = corners_original[j][0]
                        y_min = corners_original[j][1]
                pairs.append([final_res[i][0], final_res[i][1], x_min, y_min])
                temprory.append([final_res[i][0], final_res[i][1], x_min, y_min])
            pairs_inside = []
            pairs.append([final_res[index_i_min][0], final_res[index_i_min][1], temprory[index_i_max][0], temprory[index_i_max][1]])
            pairs_inside.append([final_res[0][0], final_res[0][1], temprory[index_i_min][2], temprory[index_i_min][3]])
           # pairs.append([final_res[0][0], final_res[0][1], temprory[index_i_min][2], temprory[index_i_min][3]])
            get_index_1 =index_i_max
            get_index_2 = -1
            for i in range(1, len(final_res)):
                if i != index_i_min and i != index_i_max:
                    get_index_2 = i
                    break
            pairs_inside.append([temprory[index_i_min][0], temprory[index_i_min][1], temprory[0][2], temprory[0][3]])  
            pairs_inside.append([temprory[index_i_max][0], temprory[index_i_max][1], temprory[get_index_2][2], temprory[get_index_2][3]])
            pairs_inside.append([temprory[get_index_2][0], temprory[get_index_2][1], temprory[index_i_max][2], temprory[index_i_max][3]])
            bool_ind = [0, 0, 0, 0]
            for i in range(len(corners_original)):
                bool_ind[i] = 0
                x_min = -1
                y_min = -1
                id = -1
                ro_min = 1000000
                for j in range(len(corners_original)):
                    if(j == i):
                        continue
                    if (bool_ind[j] == 1):
                        continue
                    ro_candidate = np.sqrt((corners_original[i][0] - corners_original[j][0])**2 + (corners_original[i][1] - corners_original[j][1])**2)
                    if ro_candidate < ro_min:
                        id = j
                        ro_min = ro_candidate
                        x_min = corners_original[j][0]
                        y_min = corners_original[j][1]
                bool_ind[id] = 1
                pairs.append([corners_original[i][0], corners_original[i][1], x_min, y_min])
            j = 0
            for w in range(len(pairs)):
                ed = pairs[w]
                cv2.line(clone_for_edges, (ed[0], ed[1]), (ed[2], ed[3]), color = (180, 105, 255), thickness = 2 )
                print(f"Ребро {j + 1}: ({ed[0], ed[1]}), ({ed[2], ed[3]})")
                j+=1
            for w in range(len(pairs_inside)):
                ed = pairs_inside[w]
                cv2.line(clone_for_edges, (ed[0], ed[1]), (ed[2], ed[3]), color = (255, 0, 0), thickness = 2 )
                print(f"Ребро {j + 1}: ({ed[0], ed[1]}), ({ed[2], ed[3]})")
                j+=1
            j = 0
            for i in range(len(corners_original)):
                cv2.circle(clone3, (corners_original[i][0], corners_original[i][1]), 5, (0, 255, 0), -1)
                cv2.circle(clone_image, (corners_original[i][0], corners_original[i][1]), 5, (0, 255, 0), -1)
                print(f"Вершина {j + 1}: ({corners_original[i][0], corners_original[i][1]})")
                j+=1
            for i in range(len(final_res)):
                cv2.circle(clone3, (final_res[i][0], final_res[i][1]), 5, (0, 255, 0), -1)
                cv2.circle(clone_image, (final_res[i][0], final_res[i][1]), 5, (0, 255, 0), -1)
                print(f"Вершина {j + 1}: ({final_res[i][0], final_res[i][1]})")
                j+=1
            cv2.imwrite('cube_corners.png', clone_image)
            cv2.imwrite('clone3.png', clone3)
            break
        if len(corners) ==2:
            for i in corners:
                x, y = i.ravel()
                cv2.circle(resized_cube, (int(x), int(y)), 5, (0, 255, 0), -1)
                print('loh')
                cv2.imwrite('inside_points.png', resized_cube)
                x_cube = (x / 600) * w
                y_cube = (y / 600) * h
                x_original = x_im + x_cube
                y_original = y_im + y_cube
                x_original = int(round(x_original))
                y_original = int(round(y_original))
                final_res.append([x_original, y_original])
            x_neigh = final_res[0][0]
            y_neihg = final_res[0][1]
            ro = 0
            index_i = 0
            pairs = []
            pairs.append([final_res[0][0], final_res[0][1], final_res[1][0], final_res[1][1]])
            pairs.append([final_res[0][0], final_res[0][1], final_res[1][0], final_res[1][1]])
            for i in range(len(final_res)):
                x_min, y_min =-1, -1
                ro_min = 1000000
                massiv = []
                for j in range(len(corners_original)):
                    ro_candidate = np.sqrt((final_res[i][0] - corners_original[j][0])**2 + (final_res[i][1] - corners_original[j][1])**2)
                    massiv.append([corners_original[j][0], corners_original[j][1], ro_candidate])
                massiv.sort(key=lambda x: x[2])
                pairs.append([final_res[i][0], final_res[i][1], massiv[0][0], massiv[0][1]])
                pairs.append([final_res[i][0], final_res[i][1], massiv[0][0], massiv[0][1]])
                pairs.append([final_res[i][0], final_res[i][1], massiv[1][0], massiv[1][1]])
                pairs.append([final_res[i][0], final_res[i][1], massiv[1][0], massiv[1][1]])
            for i in range(len(corners_original)):
                x_min = -1
                y_min = -1
                ro_min = 1000000
                for j in range(len(corners_original)):
                    if(j == i):
                        continue
                    ro_candidate = np.sqrt((corners_original[i][0] - corners_original[j][0])**2 + (corners_original[i][1] - corners_original[j][1])**2)
                    if ro_candidate < ro_min:
                        ro_min = ro_candidate
                        x_min = corners_original[j][0]
                        y_min = corners_original[j][1]
                pairs.append([corners_original[i][0], corners_original[i][1], x_min, y_min])
            j = 0
            for w in range(len(pairs)):
                ed = pairs[w]
                cv2.line(clone_for_edges, (ed[0], ed[1]), (ed[2], ed[3]), color = (180, 105, 255), thickness = 2 )
                print(f"Ребро {j + 1}: ({ed[0], ed[1]}), ({ed[2], ed[3]})")
                j+=1
            j = 0
            for i in range(len(corners_original)):
                cv2.circle(clone_image, (corners_original[i][0], corners_original[i][1]), 5, (0, 255, 0), -1)
                cv2.circle(clone3, (corners_original[i][0], corners_original[i][1]), 5, (0, 255, 0), -1)
                print(f"Вершина {j + 1}: ({corners_original[i][0], corners_original[i][1]})")
                j+=1
            for i in range(len(final_res)):
                cv2.circle(clone_image, (final_res[i][0], final_res[i][1]), 5, (0, 255, 0), -1)
                cv2.circle(clone3, (final_res[i][0], final_res[i][1]), 5, (0, 255, 0), -1)
                print(f"Вершина {j + 1}: ({final_res[i][0], final_res[i][1]})")
                j+=1
            for i in range(len(final_res)):
                cv2.circle(clone_image, (final_res[i][0], final_res[i][1]), 5, (0, 255, 0), -1)
                cv2.circle(clone3, (final_res[i][0], final_res[i][1]), 5, (0, 255, 0), -1)
                print(f"Вершина {j + 1}: ({final_res[i][0], final_res[i][1]})")
                j+=1
            cv2.imwrite('cube_corners.png', clone_image)
            cv2.imwrite('clone3.png', clone3)
            break          
                    
                 
            
           
    if len(approx) == 6:
        x_im, y_im, w, h = cv2.boundingRect(approx)  
        x_im = x_im - 20
        y_im = y_im - 20
        w += 40
        h +=40
        cube1 = image[y_im:y_im+h, x_im:x_im+w]
        cube = cube1
        cv2.imwrite('Вырезанная область.png', cube)
        resized_cube = cv2.resize(cube, (600, 600), interpolation=cv2.INTER_LINEAR)
        resized_cube1 = resized_cube.copy()
        resized_cube2 = resized_cube.copy()
        gray_cube = cv2.cvtColor(resized_cube, cv2.COLOR_BGR2GRAY)
        opened_image_cube = cv2.dilate(gray_cube, kernel, iterations=2)
        cv2.imwrite('dilalite_cube.png',opened_image_cube)
        eroded_image_cube = cv2.erode(opened_image_cube, kernel, iterations=3)
        cv2.imwrite('erose_cube.png', eroded_image_cube)
        blurred_cube = cv2.GaussianBlur(eroded_image_cube, (5, 5), 0)
        cv2.imwrite('cube_corners3_cube.png', blurred_cube)
        corners_original = approx.reshape(-1, 2)
        corners_cropped = corners_original - [x_im, y_im]
        scale_x = 600 / w
        scale_y = 600 / h
        corners_resized = corners_cropped * [scale_x, scale_y]
        corners_resized = np.round(corners_resized).astype(np.int32)
        exclusion_radius = 70
        mask1 = np.zeros((600, 600), dtype=np.uint8)
        cv2.drawContours(mask1, [corners_resized], -1, 255, cv2.FILLED)
        cv2.imwrite('mask.png', mask1)
        for x, y in corners_resized:
          cv2.circle(mask1, (int(x), int(y)), exclusion_radius, 0, -1)
        kernel_size = 5
        kernel = np.ones((kernel_size, kernel_size), np.uint8)
        erosion_iterations = 12
        eroded_mask = cv2.erode(mask1, kernel, iterations=erosion_iterations)
        cv2.imwrite('eroded_mask.png', eroded_mask)
        corners = cv2.goodFeaturesToTrack(blurred_cube, maxCorners=4, qualityLevel=0.1, minDistance=50, mask = eroded_mask)
        if (len(corners) == 2):
           corners = cv2.goodFeaturesToTrack(blurred_cube, maxCorners=2, qualityLevel=0.1, minDistance=70, mask = eroded_mask) 
           for i in corners:
                x, y = i.ravel()
                cv2.circle(resized_cube, (int(x), int(y)), 5, (0, 255, 0), -1)
                cv2.imwrite('inside_points.png', resized_cube)
        if (len(corners) == 3):
           corners = cv2.goodFeaturesToTrack(blurred_cube, maxCorners=2, qualityLevel=0.1, minDistance=70, mask = eroded_mask) 
           for i in corners:
                x, y = i.ravel()
                cv2.circle(resized_cube, (int(x), int(y)), 5, (0, 255, 0), -1)
                cv2.imwrite('inside_points.png', resized_cube)
        for cor in corners:
            x, y = cor.ravel()
        corners = np.int64(corners)
        x1 = -10000000
        y1 = -10000000
        x2 = -10000000
        y2 = -10000000
        if (len(corners) == 4):
            false_point = []
            for i in corners:
                x, y = i.ravel()
                false_point.append([x, y])
                cv2.circle(resized_cube, (x, y), 5, (0, 255, 0), -1)
                cv2.imwrite('inside_points.png', resized_cube)
            ros = 0
            first_x = false_point[0][0]
            first_y = false_point[0][1]
            kandidate_x = 0
            kandidate_y = 0
            index = 0
            for i in range(1, len(false_point)):
                z, m = false_point[i]
                if (np.sqrt((first_x-z)**2 + (first_y - m)**2) > ros):
                    kandidate_x = z
                    kandidate_y = m
                    ros = np.sqrt((first_x-z)**2 + (first_y - m)**2)
                    index = i
            first_pair = [[first_x, first_y]]
            second_pair = [[kandidate_x, kandidate_y]]
            hi_index = 0
            for i in range(1, len(false_point)):
                l, p = false_point[i]
                if (i != index):
                    first_pair.append([l, p])
                    hi_index = i
                    break   
            for i in range(1, len(false_point)):
                l, p = false_point[i]
                if (i != index and i != hi_index):
                    second_pair.append([l, p])
                    break
            points1 = find_true_point1(first_pair[0][0], first_pair[0][1], first_pair[1][0], first_pair[1][1], corners_resized, resized_cube2)
            x1 = points1[1][0]
            y1 = points1[1][1]
            points2 = find_true_point1(second_pair[0][0], second_pair[0][1], second_pair[1][0], second_pair[1][1], corners_resized, resized_cube2)     
            x2 = points2[1][0]
            y2 = points2[1][1]
            cv2.circle(resized_cube1, (points1[0][0], points1[0][1]), 5, (0, 255, 0), -1)  
            cv2.circle(resized_cube1, (points2[0][0], points2[0][1]), 5, (0, 255, 0), -1)
            cv2.imwrite('true_point.png', resized_cube1)
        inside_cor = []
        for i, corner in enumerate(corners):
            x_resized, y_resized = corner.ravel()
            if ((x_resized != x1 or y_resized != y1) and (x_resized != x2 or y_resized != y2)) :
                x_cube = (x_resized / 600) * w
                y_cube = (y_resized / 600) * h
                x_original = x_im + x_cube
                y_original = y_im + y_cube
                x_original = int(round(x_original))
                y_original = int(round(y_original))
                inside_cor.append([x_original, y_original])
                final.append([x_original, y_original])
                cv2.circle(clone_image, (x_original, y_original), 5, (0, 255, 0), -1)
                cv2.circle(clone3, (x_original, y_original), 5, (0, 255, 0), -1)
                continue
        cv2.imwrite('erose_cube.png', eroded_image_cube) 
        cv2.imwrite('cube_вырезанный1.png', cube)
        sort_corner = []
        edges = []
        for i in corners_original:
            x, y = i.ravel()
            sort_corner.append([x, y])
            final.append([x, y])
            cv2.circle(clone3, (x, y), 7, (0, 255, 0), -1)
            cv2.circle(clone_image, (x, y), 7, (0, 255, 0), -1)
        if (len(inside_cor)!= 1):
            for ver_x, ver_y in inside_cor:
                edge = Find(sort_corner, ver_x, ver_y)
                e = edge[2]
                edges.append([ver_x, ver_y, edge[0], edge[1]])
                x_next, y_next = sort_corner[(e+2)%6]
                x_prev, y_prev = sort_corner[(e-2)%6]
                edges.append([ver_x, ver_y, x_next, y_next])
                edges.append([ver_x, ver_y, x_prev, y_prev])
            for t in range(len(sort_corner)):
                x_s, y_s = sort_corner[t]
                x_n, y_n = sort_corner[(t+1)%6]
                edges.append([x_s, y_s, x_n, y_n])
            j = 0
            for w in range(0, 3):
                ed = edges[w]
                cv2.line(clone_for_edges, (ed[0], ed[1]), (ed[2], ed[3]), color = (180, 105, 255), thickness = 2 )
                print(f"Ребро {j + 1}: ({ed[0], ed[1]}), ({ed[2], ed[3]})")
                j+=1
            for r in range(3, 6):
                ed = edges[r]
                cv2.line(clone_for_edges, (ed[0], ed[1]), (ed[2], ed[3]), color = (0, 105, 0), thickness = 2 )
                print(f"Ребро {j + 1}: ({ed[0], ed[1]}), ({ed[2], ed[3]})")
                j+=1
            for h in range(6, 12):
                ed = edges[h]
                cv2.line(clone_for_edges, (ed[0], ed[1]), (ed[2], ed[3]), color = (0, 105, 255), thickness = 2 )
                print(f"Ребро {j + 1}: ({ed[0], ed[1]}), ({ed[2], ed[3]})")
                j+=1  
        else:
            x_only = inside_cor[0][0]
            y_only = inside_cor[0][1]
            j = 0
            for t in range(len(sort_corner)):
                x_s, y_s = sort_corner[t]
                x_n, y_n = sort_corner[(t+1)%6]
                edges.append([x_s, y_s, x_n, y_n])
                edges.append([x_s, y_s, x_only, y_only]) 
            for ed in edges:
                cv2.line(clone_for_edges, (ed[0], ed[1]), (ed[2], ed[3]), color = (0, 105, 255), thickness = 2 )
                print(f"Ребро {j + 1}: ({ed[0], ed[1]}), ({ed[2], ed[3]})")
                j+=1
                      
        cv2.imwrite('cube_corners.png', clone_image)
        cv2.imwrite('clone3.png', clone3)
        #fuck, поиск ребёр

        cv2.imwrite('kubik_with_edges.png', clone_for_edges)
        cv2.imwrite('cube_corners3.png', blurred)
        print("Изображение с отмеченными вершинами сохранено как 'cube_corners.png'.")
        break