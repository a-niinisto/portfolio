const mongoose = require('mongoose')
const supertest = require('supertest')
const app = require('../app')
const User = require('../models/user')

const api = supertest(app)

describe('tests which have no users in database before', () => {
    beforeEach(async () => {
        await User.deleteMany({})
    })
    test('password must be at least 3 chars', async () => {
        const newUser = {
            username: "Joku",
            user: "Joku",
            password: "Jo"
        }
        const response = await api.post('/api/users').send(newUser).expect(400)
        expect(response.body.error).toEqual('password must be at least 3 characters long')

        const res = await api.get('/api/users')
        const users = res.body
        expect(users).toHaveLength(0)
      })

      test('password must be defined', async () => {
        const newUser = {
            username: "Joku",
            user: "Joku"
        }
        const response = await api.post('/api/users').send(newUser).expect(400)
        expect(response.body.error).toEqual('username and password must be defined')

        const res = await api.get('/api/users')
        const users = res.body
        expect(users).toHaveLength(0)
      })

})


describe('tests which have one user in database before', () => {
    beforeEach(async () => {
        await User.deleteMany({})
        const userObject = new User({ 
            username: "Joku",
            user: "Joku",
            password: "Jo03"
        })
        await userObject.save()
    })

    test('username must be unique', async () => {
        const userObject = { 
            username: "Joku",
            user: "Joku",
            password: "Jo03"
        }
        const response = await api.post('/api/users').send(userObject).expect(400)
        expect(response.body.error).toEqual('username must be unique')

        const res = await api.get('/api/users')
        const users = res.body
        expect(users).toHaveLength(1)
    })
})