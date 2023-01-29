import { createSlice } from "@reduxjs/toolkit"


const initialState = {notification: null, timeoutID: null}

const notificationSlice = createSlice({
  name: 'notification', 
  initialState,
  reducers: {
    setNotificationTo(state, action){
      return ({notification: action.payload, timeoutID: state.timeoutID})
    },
    setTimeoutID(state, action){
      if (state.timeoutID !== null){
        clearTimeout(state.timeoutID)
      }
      return ({notification: state.notification, timeoutID: action.payload})
    }
  }
})

export const {setNotificationTo, setTimeoutID} = notificationSlice.actions

export const setNotification = (notification, time) => {
    return dispatch => {

        const timeoutID = setTimeout(() => {
            dispatch(setNotificationTo(null))
            dispatch(setTimeoutID(null))
          }, time*1000)
        dispatch(setNotificationTo(notification))
        dispatch(setTimeoutID(timeoutID))
    }
}

export default notificationSlice.reducer