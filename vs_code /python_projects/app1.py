import streamlit as st
import pandas as pd
import numpy as np
import requests
from datetime import date
import matplotlib.pyplot as plt
from datetime import datetime
import plotly.express as px
from sklearn.metrics import r2_score
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import OneHotEncoder
from sklearn.metrics import mean_squared_error
from sklearn.metrics import mean_absolute_error
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score


@st.cache_data
def load_data(file):
    data = pd.read_csv(file)
    data['timestamp'] = pd.to_datetime(data['timestamp'])
    data = data.sort_values('timestamp').copy()
    data.set_index('timestamp', inplace=True)
    data['rolling_mean'] = data['temperature'].rolling(window='30D').mean()
    data.reset_index(inplace=True)
    return data

def compute_stats(data, city):
    city_data = data[data['city'] == city]
    descriptive_stats = city_data.describe()
    season_stats = city_data.groupby('season')['temperature'].agg(['mean', 'std']).reset_index()
    return descriptive_stats, season_stats




def highlight_anomalies(data, city):
    city_data = data[data['city'] == city].copy()

    season_stats = city_data.groupby('season')['temperature'].agg(['mean', 'std']).reset_index()

    city_data = city_data.merge(season_stats, on='season', how='left', suffixes=("", "_stat"))

    city_data['anomaly'] = ~city_data.apply(lambda row: (row['mean'] - 2*row['std'] <= row['temperature'] <= row['mean'] + 2*row['std']), axis=1)
    return city_data



def get_current_temperature(city, api_key):
    url = f"http://api.openweathermap.org/data/2.5/weather?q={city}&appid={api_key}&units=metric"
    response = requests.get(url)
    data = response.json()
    if "main" in data:
        return data["main"]["temp"]
    else:
        raise Exception("Не удалось получить данные о погоде.")



def get_season(date):
    month = date.month
    if month in [12, 1, 2]:
        return 'winter'
    elif month in [3, 4, 5]:
        return 'spring'
    elif month in [6, 7, 8]:
        return 'summer'
    else:
        return 'autumn'




st.title("Анализ исторических данных температуры и мониторинг текущей погоды")

uploaded_file = st.sidebar.file_uploader("Загрузите файл с историческими данными (CSV)", type="csv")
if uploaded_file is not None:
    data = load_data(uploaded_file)

    RANDOM_STATE = 42
    X = data
    y = data['temperature']
    X_train, X_test, y_train, y_test = train_test_split(
        X, 
        y,
        random_state = RANDOM_STATE
    )
    cat_col_names = ['city', 'season']
    num_col_names = ['timestamp']

    one_hot_encoder = OneHotEncoder(drop='first', sparse_output=False, handle_unknown='ignore')

    X_train_ohe_sparse = one_hot_encoder.fit_transform(X_train[cat_col_names])
    X_test_ohe_sparse = one_hot_encoder.transform(X_test[cat_col_names])    

    encoder_col_name = one_hot_encoder.get_feature_names_out()
    scaler = StandardScaler()

    X_train_scalled = scaler.fit_transform(X_train[num_col_names])
    X_test_scalled = scaler.transform(X_test[num_col_names])

    X_train_ohe = pd.DataFrame(X_train_ohe_sparse, columns=encoder_col_name)
    X_test_ohe = pd.DataFrame(X_test_ohe_sparse, columns=encoder_col_name)    

    X_train_scalled = pd.DataFrame(X_train_scalled, columns=num_col_names)
    X_test_scalled = pd.DataFrame(X_test_scalled, columns=num_col_names)

    X_train = pd.concat([X_train_ohe, X_train_scalled], axis=1)
    X_test = pd.concat([X_test_ohe, X_test_scalled], axis=1)

    lr = LinearRegression()
    lr.fit(X_train, y_train)
    pred = lr.predict(X_test)

    MSE = mean_squared_error(y_test, pred)
    RMSE = mean_squared_error(y_test, pred)
    MAE = mean_absolute_error(y_test, pred)

    r_2 = r2_score(y_test, pred)


    dates = pd.date_range(start='2020-01-01', end='2025-12-31', freq='D')
    row = []
    cities = data['city'].unique()
    for city in cities:
        for date in dates:
            season = get_season(date)
            row.append({
                "city" : city, 
                "timestamp" : date,
                "season" : season
            })
    df_feature = pd.DataFrame(row)
    
    df_feature_ohe = one_hot_encoder.transform(df_feature[cat_col_names])
    df_feature_ohe = pd.DataFrame(df_feature_ohe, columns=encoder_col_name)


    df_feature_scalled = scaler.transform(df_feature[num_col_names])
    df_feature_scalled = pd.DataFrame(df_feature_scalled, columns=num_col_names)


    df_feature_transformed = pd.concat([df_feature_ohe, df_feature_scalled], axis=1)
    predicted_temperatures = lr.predict(df_feature_transformed)

    df_feature['predicted_temperatures'] = predicted_temperatures
    df_feature['time_stamp'] = pd.to_datetime(df_feature['timestamp'])





    selected_city = st.selectbox("Выберите город", cities)

    api_key = st.sidebar.text_input("Введите API ключ OpenWeatherMap", type="password")

    st.header("Описательная статистика")
    descriptive_stats, season_stats = compute_stats(data, selected_city)
    st.subheader("Статистика по замерам")
    st.write(descriptive_stats)
    st.sidebar.subheader("Сезонные профили (среднее и стандартное отклонение)")
    st.sidebar.write(season_stats)
    

    st.header("Временной ряд температур с выделением аномалий")
    city_data = highlight_anomalies(data, selected_city)
    

    anomalies = city_data[city_data['anomaly']]
    fig = px.line(city_data, x='timestamp', y='temperature', labels={'temperature': 'Температура (°C)', 'timestamp' : "Дата"})
    fig.add_scatter(x=city_data['timestamp'], y = city_data['rolling_mean'], mode='lines', name='Скользящее среднее', line=dict(color='orange'))
    
    fig.add_scatter(x=anomalies['timestamp'], y=anomalies['temperature'], mode = 'markers', name='Аномалии', marker=dict(color='red', size=10))
    st.plotly_chart(fig, use_container_width=True)
    

    st.header("Текущая температура")
    if api_key:
        try:
            current_temp = get_current_temperature(selected_city, api_key)
            st.write(f"Текущая температура в **{selected_city}**: **{current_temp:.1f} °C**")
            

            curr_season = 'spring'
            st.write(f"Определён текущий сезон: **{curr_season}**")
            

            seasonal_data = data[(data['city'] == selected_city) & (data['season'] == curr_season)]
            if not seasonal_data.empty:
                mean_temp = seasonal_data['temperature'].mean()
                std_temp = seasonal_data['temperature'].std()
                lower_bound = mean_temp - 2 * std_temp
                upper_bound = mean_temp + 2 * std_temp
                st.write(f"Нормальный диапазон для сезона **{curr_season}**: от **{lower_bound:.1f}°C** до **{upper_bound:.1f}°C**")
                if lower_bound <= current_temp <= upper_bound:
                    st.success("Текущая температура является нормальной для этого сезона.")
                else:
                    st.error("Текущая температура является аномальной для этого сезона!")
            else:
                st.warning("Нет исторических данных для определения нормального диапазона для выбранного сезона.")
        except Exception as e:
            st.error(f"Ошибка получения текущей погоды: {e}")
    else:
        st.info("Введите API ключ OpenWeatherMap, чтобы отобразить текущую погоду.")

    selected_city_2 = st.selectbox("Выберите город для предсказания", cities)

    df_city = df_feature[df_feature['city'] == selected_city_2].copy()
    df_city.sort_values('timestamp', inplace=True)
    fig = px.line(df_city, x='timestamp', y='predicted_temperatures', title=f'Предскеазание температуры для города {selected_city_2}', labels={'timestamp': 'Дата', 'predicted_temperatures': 'Предсказание температуры'}, markers=True)
    st.plotly_chart(fig, use_container_width=True)
    st.write("R2 LinearRegression =", r_2)
    st.write("MSE LinearRegression =", MSE)
    st.write("MAE LinearRegression =", MAE)
    st.write("RMSE LinearRegression =", RMSE)
