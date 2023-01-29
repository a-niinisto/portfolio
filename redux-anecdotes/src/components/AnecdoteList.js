import { useSelector, useDispatch } from 'react-redux'
import { setNotification } from '../reducers/notificationReducer'
import { createVote } from '../reducers/anecdoteReducer'


const AnecdoteList = () => {
    const anecdotes = useSelector(state => {
        const filter = state.filter
        const arrayForSort = [...state.anecdotes].filter(a => a.content.includes(filter))
        return (arrayForSort.sort((a1, a2) => {return (a2.votes - a1.votes)}))
    })
    const dispatch = useDispatch()

    const voteAn = (anecdote) => {
        dispatch(createVote(anecdote))
        dispatch(setNotification(`you voted '${anecdotes.find((a) => a.id === anecdote.id).content}'`, 5))
    }
    return(
        <div>
        {anecdotes.map(anecdote =>
        <div key={anecdote.id}>
          <div>
            {anecdote.content}
          </div>
          <div>
            has {anecdote.votes}
            <button onClick={() => voteAn(anecdote)}>vote</button>
          </div>
        </div>
      )}
      </div>
    )
}

export default AnecdoteList